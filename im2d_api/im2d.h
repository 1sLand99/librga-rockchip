/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd.
 * Authors:
 *  PutinLee <putin.lee@rock-chips.com>
 *  Cerf Yu <cerf.yu@rock-chips.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _im2d_h_
#define _im2d_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "rga.h"

#ifndef IM_API
#define IM_API /* define API export as needed */
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define RGA_GET_MIN(n1, n2) ((n1) < (n2) ? (n1) : (n2))

/* RGA im2d api verison */
#define RGA_API_MAJOR_VERSION	 1
#define RGA_API_MINOR_VERSION	 7
#define RGA_API_REVISION_VERSION 2
#define RGA_API_BUILD_VERSION	 11

#define RGA_API_VERSION STR(RGA_API_MAJOR_VERSION) "." STR(RGA_API_MINOR_VERSION) "." STR(RGA_API_REVISION_VERSION) "_[" STR(RGA_API_BUILD_VERSION) "]"
#define RGA_API_FULL_VERSION "rga_api version " RGA_API_VERSION

#define ERR_MSG_LEN 300

typedef enum {
    /* Rotation */
    IM_HAL_TRANSFORM_ROT_90     = 1 << 0,
    IM_HAL_TRANSFORM_ROT_180    = 1 << 1,
    IM_HAL_TRANSFORM_ROT_270    = 1 << 2,
    IM_HAL_TRANSFORM_FLIP_H     = 1 << 3,
    IM_HAL_TRANSFORM_FLIP_V     = 1 << 4,
    IM_HAL_TRANSFORM_FLIP_H_V   = 1 << 5,
    IM_HAL_TRANSFORM_MASK       = 0x3f,

    /*
     * Blend
     * Additional blend usage, can be used with both source and target configs.
     * If none of the below is set, the default "SRC over DST" is applied.
     */
    IM_ALPHA_BLEND_SRC_OVER     = 1 << 6,     /* Default, Porter-Duff "SRC over DST" */
    IM_ALPHA_BLEND_SRC          = 1 << 7,     /* Porter-Duff "SRC" */
    IM_ALPHA_BLEND_DST          = 1 << 8,     /* Porter-Duff "DST" */
    IM_ALPHA_BLEND_SRC_IN       = 1 << 9,     /* Porter-Duff "SRC in DST" */
    IM_ALPHA_BLEND_DST_IN       = 1 << 10,    /* Porter-Duff "DST in SRC" */
    IM_ALPHA_BLEND_SRC_OUT      = 1 << 11,    /* Porter-Duff "SRC out DST" */
    IM_ALPHA_BLEND_DST_OUT      = 1 << 12,    /* Porter-Duff "DST out SRC" */
    IM_ALPHA_BLEND_DST_OVER     = 1 << 13,    /* Porter-Duff "DST over SRC" */
    IM_ALPHA_BLEND_SRC_ATOP     = 1 << 14,    /* Porter-Duff "SRC ATOP" */
    IM_ALPHA_BLEND_DST_ATOP     = 1 << 15,    /* Porter-Duff "DST ATOP" */
    IM_ALPHA_BLEND_XOR          = 1 << 16,    /* Xor */
    IM_ALPHA_BLEND_MASK         = 0x1ffc0,

    IM_ALPHA_COLORKEY_NORMAL    = 1 << 17,
    IM_ALPHA_COLORKEY_INVERTED  = 1 << 18,
    IM_ALPHA_COLORKEY_MASK      = 0x60000,

    IM_SYNC                     = 1 << 19,
    IM_CROP                     = 1 << 20,    /* Unused */
    IM_COLOR_FILL               = 1 << 21,
    IM_COLOR_PALETTE            = 1 << 22,
    IM_NN_QUANTIZE              = 1 << 23,
    IM_ROP                      = 1 << 24,
    IM_ALPHA_BLEND_PRE_MUL      = 1 << 25,
    IM_ASYNC                    = 1 << 26,
    IM_MOSAIC                   = 1 << 27,
    IM_OSD                      = 1 << 28,
} IM_USAGE;

typedef enum {
    IM_RASTER_MODE              = 1 << 0,
    IM_FBC_MODE                 = 1 << 1,
    IM_TILE_MODE                = 1 << 2,
} IM_RD_MODE;

typedef enum {
    IM_SCHEDULER_RGA3_CORE0     = 1 << 0,
    IM_SCHEDULER_RGA3_CORE1     = 1 << 1,
    IM_SCHEDULER_RGA2_CORE0     = 1 << 2,
    IM_SCHEDULER_RGA3_DEFAULT   = IM_SCHEDULER_RGA3_CORE0,
    IM_SCHEDULER_RGA2_DEFAULT   = IM_SCHEDULER_RGA2_CORE0,
    IM_SCHEDULER_MASK           = 0x7,
    IM_SCHEDULER_DEFAULT        = 0,
} IM_SCHEDULER_CORE;

typedef enum {
    IM_ROP_AND                  = 0x88,
    IM_ROP_OR                   = 0xee,
    IM_ROP_NOT_DST              = 0x55,
    IM_ROP_NOT_SRC              = 0x33,
    IM_ROP_XOR                  = 0xf6,
    IM_ROP_NOT_XOR              = 0xf9,
} IM_ROP_CODE;

typedef enum {
    IM_MOSAIC_8                 = 0x0,
    IM_MOSAIC_16                = 0x1,
    IM_MOSAIC_32                = 0x2,
    IM_MOSAIC_64                = 0x3,
    IM_MOSAIC_128               = 0x4,
} IM_MOSAIC_MODE;

/* Status codes, returned by any blit function */
typedef enum {
    IM_YUV_TO_RGB_BT601_LIMIT   = 1 << 0,
    IM_YUV_TO_RGB_BT601_FULL    = 2 << 0,
    IM_YUV_TO_RGB_BT709_LIMIT   = 3 << 0,
    IM_YUV_TO_RGB_MASK          = 3 << 0,
    IM_RGB_TO_YUV_BT601_FULL    = 1 << 2,
    IM_RGB_TO_YUV_BT601_LIMIT   = 2 << 2,
    IM_RGB_TO_YUV_BT709_LIMIT   = 3 << 2,
    IM_RGB_TO_YUV_MASK          = 3 << 2,
    IM_RGB_TO_Y4                = 1 << 4,
    IM_RGB_TO_Y4_DITHER         = 2 << 4,
    IM_RGB_TO_Y1_DITHER         = 3 << 4,
    IM_Y4_MASK                  = 3 << 4,
    IM_RGB_FULL                 = 1 << 8,
    IM_RGB_CLIP                 = 2 << 8,
    IM_YUV_BT601_LIMIT_RANGE    = 3 << 8,
    IM_YUV_BT601_FULL_RANGE     = 4 << 8,
    IM_YUV_BT709_LIMIT_RANGE    = 5 << 8,
    IM_YUV_BT709_FULL_RANGE     = 6 << 8,
    IM_FULL_CSC_MASK            = 0xf << 8,
    IM_COLOR_SPACE_DEFAULT      = 0,
} IM_COLOR_SPACE_MODE;

typedef enum {
    IM_UP_SCALE,
    IM_DOWN_SCALE,
} IM_SCALE;

typedef enum {
    INTER_NEAREST,
    INTER_LINEAR,
    INTER_CUBIC,
} IM_SCALE_MODE;

typedef enum {
    IM_CONFIG_SCHEDULER_CORE,
    IM_CONFIG_PRIORITY,
    IM_CONFIG_CHECK,
} IM_CONFIG_NAME;

typedef enum {
    IM_OSD_MODE_STATISTICS      = 0x1 << 0,
    IM_OSD_MODE_AUTO_INVERT     = 0x1 << 1,
} IM_OSD_MODE;

typedef enum {
    IM_OSD_INVERT_CHANNEL_NONE          = 0x0,
    IM_OSD_INVERT_CHANNEL_Y_G           = 0x1 << 0,
    IM_OSD_INVERT_CHANNEL_C_RB          = 0x1 << 1,
    IM_OSD_INVERT_CHANNEL_ALPHA         = 0x1 << 2,
    IM_OSD_INVERT_CHANNEL_COLOR         = IM_OSD_INVERT_CHANNEL_Y_G |
                                          IM_OSD_INVERT_CHANNEL_C_RB,
    IM_OSD_INVERT_CHANNEL_BOTH          = IM_OSD_INVERT_CHANNEL_COLOR |
                                          IM_OSD_INVERT_CHANNEL_ALPHA,
} IM_OSD_INVERT_CHANNEL;

typedef enum {
    IM_OSD_FLAGS_INTERNAL = 0,
    IM_OSD_FLAGS_EXTERNAL,
} IM_OSD_FLAGS_MODE;

typedef enum {
    IM_OSD_INVERT_USE_FACTOR,
    IM_OSD_INVERT_USE_SWAP,
} IM_OSD_INVERT_MODE;

typedef enum {
    IM_OSD_BACKGROUND_DEFAULT_BRIGHT = 0,
    IM_OSD_BACKGROUND_DEFAULT_DARK,
} IM_OSD_BACKGROUND_DEFAULT;

typedef enum {
    IM_OSD_BLOCK_MODE_NORMAL = 0,
    IM_OSD_BLOCK_MODE_DIFFERENT,
} IM_OSD_BLOCK_WIDTH_MODE;

typedef enum {
    IM_OSD_MODE_HORIZONTAL,
    IM_OSD_MODE_VERTICAL,
} IM_OSD_DIRECTION;

typedef enum {
    IM_OSD_COLOR_PIXEL,
    IM_OSD_COLOR_EXTERNAL,
} IM_OSD_COLOR_MODE;

/* Get RGA basic information index */
typedef enum {
    RGA_VENDOR = 0,
    RGA_VERSION,
    RGA_MAX_INPUT,
    RGA_MAX_OUTPUT,
    RGA_BYTE_STRIDE,
    RGA_SCALE_LIMIT,
    RGA_INPUT_FORMAT,
    RGA_OUTPUT_FORMAT,
    RGA_FEATURE,
    RGA_EXPECTED,
    RGA_ALL,
} IM_INFORMATION;

/* Status codes, returned by any blit function */
typedef enum {
    IM_STATUS_NOERROR           =  2,
    IM_STATUS_SUCCESS           =  1,
    IM_STATUS_NOT_SUPPORTED     = -1,
    IM_STATUS_OUT_OF_MEMORY     = -2,
    IM_STATUS_INVALID_PARAM     = -3,
    IM_STATUS_ILLEGAL_PARAM     = -4,
    IM_STATUS_ERROR_VERSION     = -5,
    IM_STATUS_FAILED            =  0,
} IM_STATUS;

/* Rectangle definition */
typedef struct {
    int x;        /* upper-left x */
    int y;        /* upper-left y */
    int width;    /* width */
    int height;   /* height */
} im_rect;

typedef struct {
    int max;                    /* The Maximum value of the color key */
    int min;                    /* The minimum value of the color key */
} im_colorkey_range;


typedef struct im_nn {
    int scale_r;                /* scaling factor on R channal */
    int scale_g;                /* scaling factor on G channal */
    int scale_b;                /* scaling factor on B channal */
    int offset_r;               /* offset on R channal */
    int offset_g;               /* offset on G channal */
    int offset_b;               /* offset on B channal */
} im_nn_t;

/* im_info definition */
typedef struct {
    void* vir_addr;                     /* virtual address */
    void* phy_addr;                     /* physical address */
    int fd;                             /* shared fd */

    int width;                          /* width */
    int height;                         /* height */
    int wstride;                        /* wstride */
    int hstride;                        /* hstride */
    int format;                         /* format */

    int color_space_mode;               /* color_space_mode */
    int global_alpha;                   /* global_alpha */
    int rd_mode;

    /* legarcy */
    int color;                          /* color, used by color fill */
    im_colorkey_range colorkey_range;   /* range value of color key */
    im_nn_t nn;
    int rop_code;

    rga_buffer_handle_t handle;         /* buffer handle */
} rga_buffer_t;

typedef struct rga_osd_invert_factor im_osd_invert_factor_t;

typedef struct im_osd_block {
    int width_mode;             // normal or different
                                //   IM_OSD_BLOCK_MODE_NORMAL
                                //   IM_OSD_BLOCK_MODE_DIFFERENT
    union {
        int width;              // normal_mode block width
        int width_index;        // different_mode block width index in RAM
    };

    int block_count;            // block count

    int background_config;      // background config is bright or dark
                                //   IM_OSD_BACKGROUND_DEFAULT_BRIGHT
                                //   IM_OSD_BACKGROUND_DEFAULT_DARK

    int direction;              // osd block direction
                                //   IM_OSD_MODE_HORIZONTAL
                                //   IM_OSD_MODE_VERTICAL

    int color_mode;             // using src1 color or config color
                                //   IM_OSD_COLOR_PIXEL
                                //   IM_OSD_COLOR_EXTERNAL
    int background_color;       // config color: background
    int Foreground_color;       // config color: foreground
} im_osd_block_t;

typedef struct im_osd_invert {
    int invert_channel;         // invert channel config:
                                //   IM_OSD_INVERT_CHANNEL_NONE
                                //   IM_OSD_INVERT_CHANNEL_Y_G
                                //   IM_OSD_INVERT_CHANNEL_C_RB
                                //   IM_OSD_INVERT_CHANNEL_ALPHA
                                //   IM_OSD_INVERT_CHANNEL_COLOR
                                //   IM_OSD_INVERT_CHANNEL_BOTH
    int flags_mode;             // use external or inertnal RAM invert flags
                                //   IM_OSD_FLAGS_EXTERNAL
                                //   IM_OSD_FLAGS_INTERNAL
    int flags_index;            // flags index when using internal RAM invert flags

    uint64_t invert_flags;      // external invert flags
    uint64_t current_flags;     // current flags

    int invert_mode;            // invert use swap or factor
                                //   IM_OSD_INVERT_USE_FACTOR
                                //   IM_OSD_INVERT_USE_SWAP
    im_osd_invert_factor_t factor;

    int threash;
} im_osd_invert_t;

typedef struct im_osd {
    int osd_mode;                       // osd mode: statistics or auto_invert
                                        //   IM_OSD_MODE_STATISTICS
                                        //   IM_OSD_MODE_AUTO_INVERT
    im_osd_block_t block_parm;          // osd block info config

    im_osd_invert_t invert_config;
} im_osd_t;

typedef struct im_opt {
    int color;                          /* color, used by color fill */
    im_colorkey_range colorkey_range;   /* range value of color key */
    im_nn_t nn;
    int rop_code;

    int priority;
    int core;

    int mosaic_mode;

    im_osd_t osd_config;
} im_opt_t;

typedef struct im_context {
    int priority;
    IM_SCHEDULER_CORE core;
    int check_mode;
} im_context_t;

typedef struct rga_memory_parm im_handle_param_t;

/*
 * @return error message string
 */
#define imStrError(...) \
    ({ \
        const char* im2d_api_err; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            im2d_api_err = imStrError_t(IM_STATUS_INVALID_PARAM); \
        } else if (__argc == 1){ \
            im2d_api_err = imStrError_t((IM_STATUS)__args[0]); \
        } else { \
            im2d_api_err = ("Fatal error, imStrError() too many parameters\n"); \
            printf("Fatal error, imStrError() too many parameters\n"); \
        } \
        im2d_api_err; \
    })
IM_API const char* imStrError_t(IM_STATUS status);

/*
 * Import external buffers into RGA driver.
 *
 * @param fd/va/pa
 *      Select dma_fd/virtual_address/physical_address by buffer type
 * @param param
 *      Configure buffer parameters
 *
 * @return rga_buffer_handle_t
 */
IM_API rga_buffer_handle_t importbuffer_fd(int fd, im_handle_param_t *param);
IM_API rga_buffer_handle_t importbuffer_virtualaddr(void *va, im_handle_param_t *param);
IM_API rga_buffer_handle_t importbuffer_physicaladdr(uint64_t pa, im_handle_param_t *param);

/*
 * Import external buffers into RGA driver.
 *
 * @param handle
 *      rga buffer handle
 *
 * @return success or else negative error code.
 */
IM_API IM_STATUS releasebuffer_handle(rga_buffer_handle_t handle);

/*
 * Wrap image Parameters.
 *
 * @param handle
 *      RGA buffer handle.
 * @param width
 *      Width of image manipulation area.
 * @param height
 *      Height of image manipulation area.
 * @param wstride
 *      Width pixel stride, default (width = wstride).
 * @param hstride
 *      Height pixel stride, default (height = hstride).
 * @param format
 *      Image format.
 *
 * @return rga_buffer_t
 */
#define wrapbuffer_handle(handle, width, height, format, ...) \
    ({ \
        rga_buffer_t im2d_api_buffer; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            im2d_api_buffer = wrapbuffer_handle_t(handle, width, height, width, height, format); \
        } else if (__argc == 2){ \
            im2d_api_buffer = wrapbuffer_handle_t(handle, width, height, __args[0], __args[1], format); \
        } else { \
            printf("invalid parameter\n"); \
        } \
        im2d_api_buffer; \
    })
IM_API rga_buffer_t wrapbuffer_handle_t(rga_buffer_handle_t handle, int width, int height, int wstride, int hstride, int format);

/* For legarcy. */
#define wrapbuffer_virtualaddr(vir_addr, width, height, format, ...) \
    ({ \
        rga_buffer_t im2d_api_buffer; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            im2d_api_buffer = wrapbuffer_virtualaddr_t(vir_addr, width, height, width, height, format); \
        } else if (__argc == 2){ \
            im2d_api_buffer = wrapbuffer_virtualaddr_t(vir_addr, width, height, __args[0], __args[1], format); \
        } else { \
            printf("invalid parameter\n"); \
        } \
        im2d_api_buffer; \
    })

#define wrapbuffer_physicaladdr(phy_addr, width, height, format, ...) \
    ({ \
        rga_buffer_t im2d_api_buffer; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            im2d_api_buffer = wrapbuffer_physicaladdr_t(phy_addr, width, height, width, height, format); \
        } else if (__argc == 2){ \
            im2d_api_buffer = wrapbuffer_physicaladdr_t(phy_addr, width, height, __args[0], __args[1], format); \
        } else { \
            printf("invalid parameter\n"); \
        } \
        im2d_api_buffer; \
    })

#define wrapbuffer_fd(fd, width, height, format, ...) \
    ({ \
        rga_buffer_t im2d_api_buffer; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            im2d_api_buffer = wrapbuffer_fd_t(fd, width, height, width, height, format); \
        } else if (__argc == 2){ \
            im2d_api_buffer = wrapbuffer_fd_t(fd, width, height, __args[0], __args[1], format); \
        } else { \
            printf("invalid parameter\n"); \
        } \
        im2d_api_buffer; \
    })
IM_API rga_buffer_t wrapbuffer_virtualaddr_t(void* vir_addr, int width, int height, int wstride, int hstride, int format);
IM_API rga_buffer_t wrapbuffer_physicaladdr_t(void* phy_addr, int width, int height, int wstride, int hstride, int format);
IM_API rga_buffer_t wrapbuffer_fd_t(int fd, int width, int height, int wstride, int hstride, int format);

/*
 * Query RGA basic information, supported resolution, supported format, etc.
 *
 * @param name
 *      RGA_VENDOR
 *      RGA_VERSION
 *      RGA_MAX_INPUT
 *      RGA_MAX_OUTPUT
 *      RGA_INPUT_FORMAT
 *      RGA_OUTPUT_FORMAT
 *      RGA_EXPECTED
 *      RGA_ALL
 *
 * @returns a string describing properties of RGA.
 */
IM_API const char* querystring(int name);

/*
 * check RGA basic information, supported resolution, supported format, etc.
 *
 * @param src
 * @param dst
 * @param src_rect
 * @param dst_rect
 * @param mode_usage
 *
 * @returns no error or else negative error code.
 */
#define imcheck(src, dst, src_rect, dst_rect, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_NOERROR; \
        rga_buffer_t __pat; \
        im_rect __pat_rect; \
        memset(&__pat, 0, sizeof(rga_buffer_t)); \
        memset(&__pat_rect, 0, sizeof(im_rect)); \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            rga_check_perpare((rga_buffer_t *)(&src), (rga_buffer_t *)(&dst), (rga_buffer_t *)(&__pat), \
                              (im_rect *)(&src_rect), (im_rect *)(&dst_rect), (im_rect *)(&__pat_rect), 0); \
            __ret = imcheck_t(src, dst, __pat, src_rect, dst_rect, __pat_rect, 0); \
        } else if (__argc == 1){ \
            rga_check_perpare((rga_buffer_t *)(&src), (rga_buffer_t *)(&dst), (rga_buffer_t *)(&__pat), \
                              (im_rect *)(&src_rect), (im_rect *)(&dst_rect), (im_rect *)(&__pat_rect), __args[0]); \
            __ret = imcheck_t(src, dst, __pat, src_rect, dst_rect, __pat_rect, __args[0]); \
        } else { \
            __ret = IM_STATUS_FAILED; \
            printf("check failed\n"); \
        } \
        __ret; \
    })
#define imcheck_composite(src, dst, pat, src_rect, dst_rect, pat_rect, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_NOERROR; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            rga_check_perpare((rga_buffer_t *)(&src), (rga_buffer_t *)(&dst), (rga_buffer_t *)(&pat), \
                              (im_rect *)(&src_rect), (im_rect *)(&dst_rect), (im_rect *)(&pat_rect), 0); \
            __ret = imcheck_t(src, dst, pat, src_rect, dst_rect, pat_rect, 0); \
        } else if (__argc == 1){ \
            rga_check_perpare((rga_buffer_t *)(&src), (rga_buffer_t *)(&dst), (rga_buffer_t *)(&pat), \
                              (im_rect *)(&src_rect), (im_rect *)(&dst_rect), (im_rect *)(&pat_rect), __args[0]); \
            __ret = imcheck_t(src, dst, pat, src_rect, dst_rect, pat_rect, __args[0]); \
        } else { \
            __ret = IM_STATUS_FAILED; \
            printf("check failed\n"); \
        } \
        __ret; \
    })
IM_API void rga_check_perpare(rga_buffer_t *src, rga_buffer_t *dst, rga_buffer_t *pat,
                              im_rect *src_rect, im_rect *dst_rect, im_rect *pat_rect, int mode_usage);
IM_API IM_STATUS imcheck_t(const rga_buffer_t src, const rga_buffer_t dst, const rga_buffer_t pat,
                           const im_rect src_rect, const im_rect dst_rect, const im_rect pat_rect, const int mode_usage);

/*
 * Resize
 *
 * @param src
 * @param dst
 * @param fx
 * @param fy
 * @param interpolation
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imresize(src, dst, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        double __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(double); \
        if (__argc == 0) { \
            __ret = imresize_t(src, dst, 0, 0, INTER_LINEAR, 1); \
        } else if (__argc == 2){ \
            __ret = imresize_t(src, dst, __args[RGA_GET_MIN(__argc, 0)], __args[RGA_GET_MIN(__argc, 1)], INTER_LINEAR, 1); \
        } else if (__argc == 3){ \
            __ret = imresize_t(src, dst, __args[RGA_GET_MIN(__argc, 0)], __args[RGA_GET_MIN(__argc, 1)], (int)__args[RGA_GET_MIN(__argc, 2)], 1); \
        } else if (__argc == 4){ \
            __ret = imresize_t(src, dst, __args[RGA_GET_MIN(__argc, 0)], __args[RGA_GET_MIN(__argc, 1)], (int)__args[RGA_GET_MIN(__argc, 2)], (int)__args[RGA_GET_MIN(__argc, 3)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })

#define impyramid(src, dst, direction) \
        imresize_t(src, \
                   dst, \
                   direction == IM_UP_SCALE ? 0.5 : 2, \
                   direction == IM_UP_SCALE ? 0.5 : 2, \
                   INTER_LINEAR, 1)

IM_API IM_STATUS imresize_t(const rga_buffer_t src, rga_buffer_t dst, double fx, double fy, int interpolation, int sync);

/*
 * Crop
 *
 * @param src
 * @param dst
 * @param rect
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imcrop(src, dst, rect, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imcrop_t(src, dst, rect, 1); \
        } else if (__argc == 1){ \
            __ret = imcrop_t(src, dst, rect, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })

IM_API IM_STATUS imcrop_t(const rga_buffer_t src, rga_buffer_t dst, im_rect rect, int sync);

/*
 * rotation
 *
 * @param src
 * @param dst
 * @param rotation
 *      IM_HAL_TRANSFORM_ROT_90
 *      IM_HAL_TRANSFORM_ROT_180
 *      IM_HAL_TRANSFORM_ROT_270
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imrotate(src, dst, rotation, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imrotate_t(src, dst, rotation, 1); \
        } else if (__argc == 1){ \
            __ret = imrotate_t(src, dst, rotation, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })

IM_API IM_STATUS imrotate_t(const rga_buffer_t src, rga_buffer_t dst, int rotation, int sync);

/*
 * flip
 *
 * @param src
 * @param dst
 * @param mode
 *      IM_HAL_TRANSFORM_FLIP_H
 *      IM_HAL_TRANSFORM_FLIP_V
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imflip(src, dst, mode, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imflip_t(src, dst, mode, 1); \
        } else if (__argc == 1){ \
            __ret = imflip_t(src, dst, mode, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })

IM_API IM_STATUS imflip_t (const rga_buffer_t src, rga_buffer_t dst, int mode, int sync);

/*
 * fill/reset/draw
 *
 * @param src
 * @param dst
 * @param rect
 * @param color
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imfill(buf, rect, color, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imfill_t(buf, rect, color, 1); \
        } else if (__argc == 1){ \
            __ret = imfill_t(buf, rect, color, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })

#define imreset(buf, rect, color, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imfill_t(buf, rect, color, 1); \
        } else if (__argc == 1){ \
            __ret = imfill_t(buf, rect, color, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })

#define imdraw(buf, rect, color, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imfill_t(buf, rect, color, 1); \
        } else if (__argc == 1){ \
            __ret = imfill_t(buf, rect, color, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })
IM_API IM_STATUS imfill_t(rga_buffer_t dst, im_rect rect, int color, int sync);

/*
 * palette
 *
 * @param src
 * @param dst
 * @param lut
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define impalette(src, dst, lut,  ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = impalette_t(src, dst, lut, 1); \
        } else if (__argc == 1){ \
            __ret = impalette_t(src, dst, lut, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })
IM_API IM_STATUS impalette_t(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t lut, int sync);

/*
 * translate
 *
 * @param src
 * @param dst
 * @param x
 * @param y
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imtranslate(src, dst, x, y, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imtranslate_t(src, dst, x, y, 1); \
        } else if (__argc == 1){ \
            __ret = imtranslate_t(src, dst, x, y, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })
IM_API IM_STATUS imtranslate_t(const rga_buffer_t src, rga_buffer_t dst, int x, int y, int sync);

/*
 * copy
 *
 * @param src
 * @param dst
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imcopy(src, dst, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imcopy_t(src, dst, 1); \
        } else if (__argc == 1){ \
            __ret = imcopy_t(src, dst, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })

IM_API IM_STATUS imcopy_t(const rga_buffer_t src, rga_buffer_t dst, int sync);

/*
 * blend (SRC + DST -> DST or SRCA + SRCB -> DST)
 *
 * @param srcA
 * @param srcB can be NULL.
 * @param dst
 * @param mode
 *      IM_ALPHA_BLEND_MODE
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imblend(srcA, dst, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        rga_buffer_t srcB; \
        memset(&srcB, 0x00, sizeof(rga_buffer_t)); \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imblend_t(srcA, srcB, dst, IM_ALPHA_BLEND_SRC_OVER, 1); \
        } else if (__argc == 1){ \
            __ret = imblend_t(srcA, srcB, dst, (int)__args[RGA_GET_MIN(__argc, 0)], 1); \
        } else if (__argc == 2){ \
            __ret = imblend_t(srcA, srcB, dst, (int)__args[RGA_GET_MIN(__argc, 0)], (int)__args[RGA_GET_MIN(__argc, 1)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })
#define imcomposite(srcA, srcB, dst, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imblend_t(srcA, srcB, dst, IM_ALPHA_BLEND_SRC_OVER, 1); \
        } else if (__argc == 1){ \
            __ret = imblend_t(srcA, srcB, dst, (int)__args[RGA_GET_MIN(__argc, 0)], 1); \
        } else if (__argc == 2){ \
            __ret = imblend_t(srcA, srcB, dst, (int)__args[RGA_GET_MIN(__argc, 0)], (int)__args[RGA_GET_MIN(__argc, 1)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })
IM_API IM_STATUS imblend_t(const rga_buffer_t srcA, const rga_buffer_t srcB, rga_buffer_t dst, int mode, int sync);

/*
 * color key
 *
 * @param src
 * @param dst
 * @param colorkey_range
 *      max color
 *      min color
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imcolorkey(src, dst, range, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imcolorkey_t(src, dst, range, IM_ALPHA_COLORKEY_NORMAL, 1); \
        } else if (__argc == 1){ \
            __ret = imcolorkey_t(src, dst, range, (int)__args[RGA_GET_MIN(__argc, 0)], 1); \
        } else if (__argc == 2){ \
            __ret = imcolorkey_t(src, dst, range, (int)__args[RGA_GET_MIN(__argc, 0)], (int)__args[RGA_GET_MIN(__argc, 1)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })
IM_API IM_STATUS imcolorkey_t(const rga_buffer_t src, rga_buffer_t dst, im_colorkey_range range, int mode, int sync);

/*
 * format convert
 *
 * @param src
 * @param dst
 * @param sfmt
 * @param dfmt
 * @param mode
 *      color space mode: IM_COLOR_SPACE_MODE
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imcvtcolor(src, dst, sfmt, dfmt, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imcvtcolor_t(src, dst, sfmt, dfmt, IM_COLOR_SPACE_DEFAULT, 1); \
        } else if (__argc == 1){ \
            __ret = imcvtcolor_t(src, dst, sfmt, dfmt, (int)__args[RGA_GET_MIN(__argc, 0)], 1); \
        } else if (__argc == 2){ \
            __ret = imcvtcolor_t(src, dst, sfmt, dfmt, (int)__args[RGA_GET_MIN(__argc, 0)], (int)__args[RGA_GET_MIN(__argc, 1)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })

IM_API IM_STATUS imcvtcolor_t(rga_buffer_t src, rga_buffer_t dst, int sfmt, int dfmt, int mode, int sync);

/*
 * nn quantize
 *
 * @param src
 * @param dst
 * @param nninfo
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imquantize(src, dst, nn_info, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imquantize_t(src, dst, nn_info, 1); \
        } else if (__argc == 1){ \
            __ret = imquantize_t(src, dst, nn_info, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })

IM_API IM_STATUS imquantize_t(const rga_buffer_t src, rga_buffer_t dst, im_nn_t nn_info, int sync);

/*
 * ROP
 *
 * @param src
 * @param dst
 * @param rop_code
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imrop(src, dst, rop_code, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imrop_t(src, dst, rop_code, 1); \
        } else if (__argc == 1){ \
            __ret = imrop_t(src, dst, rop_code, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })
IM_API IM_STATUS imrop_t(const rga_buffer_t src, rga_buffer_t dst, int rop_code, int sync);

/*
 * MOSAIC
 *
 * @param src
 * @param dst
 * @param mosaic_mode
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define immosaic(src, dst, mosaic_mode, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = immosaic_t(src, dst, mosaic_mode, 1); \
        } else if (__argc == 1){ \
            __ret = immosaic_t(src, dst, mosaic_mode, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })
IM_API IM_STATUS immosaic_t(const rga_buffer_t src, rga_buffer_t dst, int mosaic_mode, int sync);

/*
 * OSD
 *
 * @param osd
 *      osd block
 * @param dst
 *      background image
 * @param osd_rect
 * @param osd_config
 *      osd mode config
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imosd(osd, dst, osd_rect, osd_config, ...) \
    ({ \
        IM_STATUS __ret = IM_STATUS_SUCCESS; \
        int __args[] = {__VA_ARGS__}; \
        int __argc = sizeof(__args)/sizeof(int); \
        if (__argc == 0) { \
            __ret = imosd_t(osd, dst, osd_rect, osd_config, 1); \
        } else if (__argc == 1){ \
            __ret = imosd_t(osd, dst, osd_rect, osd_config, (int)__args[RGA_GET_MIN(__argc, 0)]); \
        } else { \
            __ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        __ret; \
    })
IM_API IM_STATUS imosd_t(const rga_buffer_t osd,const rga_buffer_t dst,
                         const im_rect osd_rect, im_osd_t *osd_config, int sync);

/*
 * process
 *
 * @param src
 * @param dst
 * @param usage
 * @param ...
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
IM_API IM_STATUS improcess(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t pat,
                        im_rect srect, im_rect drect, im_rect prect, int usage);

/*
 * block until all execution is complete
 *
 * @returns success or else negative error code.
 */
IM_API IM_STATUS imsync(int out_fence_fd);

/*
 * config
 *
 * @param name
 *      enum IM_CONFIG_NAME
 * @param value
 *
 * @returns success or else negative error code.
 */
IM_API IM_STATUS imconfig(IM_CONFIG_NAME name, uint64_t value);

#ifdef __cplusplus
}
#endif
#endif /* _im2d_h_ */

