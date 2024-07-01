#ifndef __LV_ANIM_LIGHT_H__
#define __LV_ANIM_LIGHT_H__

#include <stdint.h>

struct _lv_anim_t;
struct _lv_timer_t;

/** Get the current value during an animation*/
typedef int32_t (*lv_anim_path_cb_t)(const struct _lv_anim_t *);

/** Generic prototype of "animator" functions.
 * First parameter is the variable to animate.
 * Second parameter is the value to set.
 * Compatible with `lv_xxx_set_yyy(obj, value)` functions
 * The `x` in `_xcb_t` means it's not a fully generic prototype because
 * it doesn't receive `lv_anim_t *` as its first argument*/
typedef void (*lv_anim_exec_xcb_t)(void *, int32_t);

/** Callback to call when the animation is ready*/
typedef void (*lv_anim_ready_cb_t)(struct _lv_anim_t *);

/** Describes an animation*/
typedef struct _lv_anim_t {
    lv_anim_exec_xcb_t exec_cb;          /**< Function to execute to animate*/
    lv_anim_ready_cb_t ready_cb;         /**< Call it when the animation is ready*/
    lv_anim_path_cb_t path_cb;         /**< Describe the path (curve) of animations*/
    int32_t start_value;               /**< Start value*/
    int32_t current_value;             /**< Current value*/
    int32_t end_value;                 /**< End value*/
    int32_t time;                /**< Animation time in ms*/
    int32_t act_time;            /**< Current time in animation. Set to negative to make delay.*/
} lv_anim_t;

#define LV_BEZIER_VAL_MAX 1024 /**< Max time in Bezier functions (not [0..1] to use integers)*/
#define LV_BEZIER_VAL_SHIFT 10 /**< log2(LV_BEZIER_VAL_MAX): used to normalize up scaled values*/

#define LV_ANIM_RESOLUTION 1024
#define LV_ANIM_RES_SHIFT 10


/**
 * Calculate the current value of an animation applying linear characteristic
 * @param a     pointer to an animation
 * @return      the current value to set
 */
int32_t lv_anim_path_linear(const lv_anim_t * a);

/**
 * Calculate the current value of an animation slowing down the start phase
 * @param a     pointer to an animation
 * @return      the current value to set
 */
int32_t lv_anim_path_ease_in(const lv_anim_t * a);

/**
 * Calculate the current value of an animation slowing down the end phase
 * @param a     pointer to an animation
 * @return      the current value to set
 */
int32_t lv_anim_path_ease_out(const lv_anim_t * a);

/**
 * Calculate the current value of an animation applying an "S" characteristic (cosine)
 * @param a     pointer to an animation
 * @return      the current value to set
 */
int32_t lv_anim_path_ease_in_out(const lv_anim_t * a);

/**
 * Calculate the current value of an animation with overshoot at the end
 * @param a     pointer to an animation
 * @return      the current value to set
 */
int32_t lv_anim_path_overshoot(const lv_anim_t * a);

/**
 * Calculate the current value of an animation with 3 bounces
 * @param a     pointer to an animation
 * @return      the current value to set
 */
int32_t lv_anim_path_bounce(const lv_anim_t * a);

/**
 * Calculate the current value of an animation applying step characteristic.
 * (Set end value on the end of the animation)
 * @param a     pointer to an animation
 * @return      the current value to set
 */
int32_t lv_anim_path_step(const lv_anim_t * a);

void lv_anim_run(lv_anim_t* a);

lv_anim_t lv_anim_start(uint32_t end, int32_t start, lv_anim_exec_xcb_t exec_cb, uint32_t duration);

#endif
