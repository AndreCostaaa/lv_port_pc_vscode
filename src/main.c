/**
 * @file main.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include <SDL.h>

#include "hal/hal.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static lv_obj_t *rect;
static lv_obj_t *opacity_label;
static lv_obj_t *slider;

static void slider_event_cb(lv_event_t *e)
{
	lv_obj_t *slider = lv_event_get_target(e);
	int32_t value = lv_slider_get_value(slider);

	lv_opa_t opacity = (lv_opa_t)((value * 255) / 100);
	lv_obj_set_style_bg_opa(rect, opacity, 0);

	static char buf[16];
	lv_snprintf(buf, sizeof(buf), "Opacity: %d%%", (int)value);
	lv_label_set_text(opacity_label, buf);
}

void create_opacity_app(void)
{
	lv_obj_t *main_cont = lv_obj_create(lv_screen_active());
	lv_obj_set_size(main_cont, 300, 400);
	lv_obj_center(main_cont);
	lv_obj_set_style_bg_color(main_cont, lv_color_hex(0xf0f0f0), 0);

	lv_obj_t *other_rect = lv_obj_create(main_cont);
	lv_obj_set_size(other_rect, 100, 50);
	lv_obj_set_pos(other_rect, 100, 75);
	lv_obj_set_style_bg_color(other_rect, lv_color_hex(0xaf54287), 0);
	lv_obj_set_style_bg_opa(other_rect, LV_OPA_100, 0);

	rect = lv_obj_create(main_cont);
	lv_obj_set_size(rect, 200, 100);
	lv_obj_set_pos(rect, 50, 50);
	lv_obj_set_style_bg_color(rect, lv_color_hex(0x4287f5), 0);
	lv_obj_set_style_bg_opa(rect, LV_OPA_100, 0);
	lv_obj_set_style_radius(rect, 8, 0);

	opacity_label = lv_label_create(main_cont);
	lv_label_set_text(opacity_label, "Opacity: 100%");
	lv_obj_set_pos(opacity_label, 50, 170);
	lv_obj_set_style_text_font(opacity_label, &lv_font_montserrat_16, 0);
	lv_obj_set_style_text_color(opacity_label, lv_color_hex(0x333333), 0);

	slider = lv_slider_create(main_cont);
	lv_obj_set_size(slider, 200, 20);
	lv_obj_set_pos(slider, 50, 220);
	lv_slider_set_range(slider, 0, 100);
	lv_slider_set_value(slider, 100, LV_ANIM_OFF); // Start at 100%
	lv_obj_set_style_bg_color(slider, lv_color_hex(0xdddddd), LV_PART_MAIN);
	lv_obj_set_style_bg_color(slider, lv_color_hex(0x4287f5), LV_PART_INDICATOR);
	lv_obj_set_style_bg_color(slider, lv_color_hex(0x1a73e8), LV_PART_KNOB);

	lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

#if LV_USE_OS != LV_OS_FREERTOS

int main(int argc, char **argv)
{
	(void)argc; /*Unused*/
	(void)argv; /*Unused*/

	/*Initialize LVGL*/
	lv_init();

	/*Initialize the HAL (display, input devices, tick) for LVGL*/
	sdl_hal_init(320, 480);

	/* Run the default demo */
	/* To try a different demo or example, replace this with one of: */
	/* - lv_demo_benchmark(); */
	/* - lv_demo_stress(); */
	/* - lv_example_label_1(); */
	/* - etc. */
	create_opacity_app();

	while (1) {
		/* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
		uint32_t sleep_time_ms = lv_timer_handler();
		if (sleep_time_ms == LV_NO_TIMER_READY) {
			sleep_time_ms = LV_DEF_REFR_PERIOD;
		}
#ifdef _MSC_VER
		Sleep(sleep_time_ms);
#else
		usleep(sleep_time_ms * 1000);
#endif
	}

	return 0;
}

#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/
