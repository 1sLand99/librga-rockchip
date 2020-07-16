/*
 * Copyright (C) 2016 Rockchip Electronics Co.Ltd
 * Authors:
 *  Zhiqin Wei <wzq@rock-chips.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#define LOG_NDEBUG 0
#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "rockchiprga"
#endif

#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include <fcntl.h>

#include <signal.h>
#include <time.h>

#ifdef ANDROID
#include <utils/misc.h>
#include <cutils/properties.h>

#ifndef ANDROID_8

#include <binder/IPCThreadState.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

#endif

#include <utils/Atomic.h>
#include <utils/Errors.h>
#include <android/log.h>
#include <utils/Log.h>
#include <log/log_main.h>
#include <utils/Mutex.h>
#include <utils/Singleton.h>

#include <ui/PixelFormat.h>
#include <ui/Rect.h>
#include <ui/Region.h>
#include <ui/DisplayInfo.h>
#include <ui/GraphicBufferMapper.h>

#include <gui/ISurfaceComposer.h>

#endif

#include "RockchipRga.h"

#include "normal/NormalRga.h"

#ifdef LINUX
#if LIBDRM
#include <drm.h>
#include "drm_mode.h"
#include "xf86drm.h"
#endif
#endif

#define RK_GRAPHICS_VER "version:1.00"
#ifdef ANDROID
namespace android {
// ---------------------------------------------------------------------------
    ANDROID_SINGLETON_STATIC_INSTANCE(RockchipRga)
#else
RGA_SINGLETON_STATIC_INSTANCE(RockchipRga)
#endif

    RockchipRga::RockchipRga():
        mSupportRga(false),
        mLogOnce(0),
        mLogAlways(0),
        mContext(NULL) {
        RkRgaInit();
        ALOGE("Rga built version:%s", RK_GRAPHICS_VER);
    }

    RockchipRga::~RockchipRga() {
        RgaDeInit(mContext);
    }

    int RockchipRga::RkRgaInit() {
        int ret = 0;

        if (mSupportRga)
            return 0;

        ret = RgaInit(&mContext);
        if(ret == 0)
            mSupportRga = true;
        else
            mSupportRga = false;

        return ret;
    }

#ifdef LINUX
    void RockchipRga::RkRgaDeInit() {
        if (mSupportRga)
            RgaDeInit(mContext);

        mSupportRga = false;
    }

    int RockchipRga::RkRgaAllocBuffer(int drm_fd, bo_t *bo_info, int width,
                                      int height, int bpp) {
#if LIBDRM
        struct drm_mode_create_dumb arg;
        int ret;

        memset(&arg, 0, sizeof(arg));
        arg.bpp = bpp;
        arg.width = width;
        arg.height = height;

        ret = drmIoctl(drm_fd, DRM_IOCTL_MODE_CREATE_DUMB, &arg);
        if (ret) {
            fprintf(stderr, "failed to create dumb buffer: %s\n", strerror(errno));
            return ret;
        }

        bo_info->handle = arg.handle;
        bo_info->size = arg.size;
        bo_info->pitch = arg.pitch;

        return 0;
#else
        return -1;
#endif
    }

    int RockchipRga::RkRgaFreeBuffer(int drm_fd, bo_t *bo_info) {
#if LIBDRM
        struct drm_mode_destroy_dumb arg;
        int ret;

        if (bo_info->handle <= 0)
            return -EINVAL;
        memset(&arg, 0, sizeof(arg));
        arg.handle = bo_info->handle;
        ret = drmIoctl(drm_fd, DRM_IOCTL_MODE_DESTROY_DUMB, &arg);
        if (ret) {
            fprintf(stderr, "failed to destroy dumb buffer: %s\n", strerror(errno));
            return -errno;
        }
        bo_info->handle = 0;

        return 0;
#else
        return -1;
#endif
    }

    int RockchipRga::RkRgaGetAllocBuffer(bo_t *bo_info, int width, int height, int bpp) {
        static const char* card = "/dev/dri/card0";
        int ret;
        int drm_fd;
        int flag = O_RDWR;
#ifdef O_CLOEXEC
        flag |= O_CLOEXEC;
#endif
        bo_info->fd = -1;
        bo_info->handle = 0;
        drm_fd = open(card, flag);
        if (drm_fd < 0) {
            fprintf(stderr, "Fail to open %s: %m\n", card);
            return -errno;
        }
        ret = RkRgaAllocBuffer(drm_fd, bo_info, width, height, bpp);
        if (ret) {
            close(drm_fd);
            return ret;
        }
        bo_info->fd = drm_fd;
        return 0;
    }

    int RockchipRga::RkRgaGetMmap(bo_t *bo_info) {
#if LIBDRM
        struct drm_mode_map_dumb arg;
        void *map;
        int ret;

        memset(&arg, 0, sizeof(arg));
        arg.handle = bo_info->handle;
        ret = drmIoctl(bo_info->fd, DRM_IOCTL_MODE_MAP_DUMB, &arg);
        if (ret)
            return ret;
        map = mmap64(0, bo_info->size, PROT_READ | PROT_WRITE, MAP_SHARED, bo_info->fd, arg.offset);
        if (map == MAP_FAILED)
            return -EINVAL;
        bo_info->ptr = map;
        return 0;
#else
        return -1;
#endif
    }

    int RockchipRga::RkRgaUnmap(bo_t *bo_info) {
        munmap(bo_info->ptr, bo_info->size);
        bo_info->ptr = NULL;
        return 0;
    }

    int RockchipRga::RkRgaFree(bo_t *bo_info) {
        int ret;
        if (bo_info->fd < 0)
            return -EINVAL;
        ret = RkRgaFreeBuffer(bo_info->fd, bo_info);
        close(bo_info->fd);
        bo_info->fd = -1;
        return ret;
    }

    int RockchipRga::RkRgaGetBufferFd(bo_t *bo_info, int *fd) {
#if LIBDRM
        int ret = 0;
        ret = drmPrimeHandleToFD(bo_info->fd, bo_info->handle, 0, fd);
        return ret;
#else
        return -1;
#endif
    }
#endif

#ifdef ANDROID
    int RockchipRga::RkRgaGetBufferFd(buffer_handle_t handle, int *fd) {
        int ret = 0;
        ret = RkRgaGetHandleFd(handle, fd);
        return ret;
    }
#endif

    int RockchipRga::RkRgaBlit(rga_info *src, rga_info *dst, rga_info *src1) {
        int ret = 0;
        ret = RgaBlit(src, dst, src1);
        if (ret) {
            RkRgaLogOutUserPara(src);
            RkRgaLogOutUserPara(dst);
            RkRgaLogOutUserPara(src1);
            ALOGE("This output the user patamaters when rga call blit fail");
        }
        return ret;
    }

    int RockchipRga::RkRgaFlush() {
        int ret = 0;
        ret = RgaFlush();
        if (ret) {
            ALOGE("RgaFlush Failed");
        }
        return ret;
    }

    int RockchipRga::RkRgaCollorFill(rga_info *dst) {
        int ret = 0;
        ret = RgaCollorFill(dst);
        return ret;
    }

    int RockchipRga::RkRgaLogOutUserPara(rga_info *rgaInfo) {
        if (!rgaInfo)
            return -EINVAL;

        ALOGE("fd-vir-phy-hnd-format[%d, %p, %p, %p, %d]", rgaInfo->fd,
              rgaInfo->virAddr, rgaInfo->phyAddr, (void*)rgaInfo->hnd, rgaInfo->format);
        ALOGE("rect[%d, %d, %d, %d, %d, %d, %d, %d]",
              rgaInfo->rect.xoffset, rgaInfo->rect.yoffset,
              rgaInfo->rect.width,   rgaInfo->rect.height, rgaInfo->rect.wstride,
              rgaInfo->rect.hstride, rgaInfo->rect.format, rgaInfo->rect.size);
        ALOGE("f-blend-size-rotation-col-log-mmu[%d, %x, %d, %d, %d, %d, %d]",
              rgaInfo->format, rgaInfo->blend, rgaInfo->bufferSize,
              rgaInfo->rotation, rgaInfo->color, rgaInfo->testLog, rgaInfo->mmuFlag);
        return 0;
    }

// ---------------------------------------------------------------------------
#ifdef ANDROID
}; // namespace android
#endif

