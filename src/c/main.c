//----------------------------------------------------------------------------
//
//  Pebble smartwatch side of Project 20.1 for "Learning C with Pebble"
//
//  Mike Jipping, August, 2016

#include <pebble.h>

static Window *window;	
static TextLayer *random_text_layer;
static TextLayer *city_text_layer;
static AppTimer *random_timer;
static AppTimer *city_timer;
	
// Called when a message is received from the JavaScript side
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
    
	tuple = dict_find(received, MESSAGE_KEY_RANDOM);
	if(tuple) {
        text_layer_set_text(random_text_layer, tuple->value->cstring);
	} else {
        text_layer_set_text(random_text_layer, "ERROR!");
    }
  tuple = dict_find(received, MESSAGE_KEY_CITY);
  if (tuple) {
        text_layer_set_text(city_text_layer, tuple->value->cstring);
  } else {
        text_layer_set_text(city_text_layer, "ERROR!");
  }
}

// Make a request for the random number
static void ask_for_random(void *context) {
    DictionaryIterator *iter;
	
	uint32_t result = app_message_outbox_begin(&iter);
    if (result == APP_MSG_OK) {
	    dict_write_int8(iter, MESSAGE_KEY_ASK, 1);  // Gotta have a payload
	    dict_write_end(iter);
        app_message_outbox_send();
    }
    random_timer = app_timer_register(5000, ask_for_random, NULL);
}

// Make a request for the city
static void ask_for_city(void *context) {
   DictionaryIterator *iter;
  
   uint32_t result = app_message_outbox_begin(&iter);
  
   if (result == APP_MSG_OK) {
     dict_write_int8(iter, MESSAGE_KEY_CITY, 1);
     dict_write_end(iter);
     app_message_outbox_send();
   }
   city_timer = app_timer_register(10000, ask_for_city, NULL);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    app_timer_cancel(random_timer);
    ask_for_random(NULL);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void init(void) {
    window = window_create();

    Layer *window_layer = window_get_root_layer(window);
    GRect frame = layer_get_frame(window_layer);

    window_set_click_config_provider(window, click_config_provider);
    const bool animated = true;
    window_stack_push(window, animated);

    // Register AppMessage handlers
	  app_message_register_inbox_received(in_received_handler); 

    // Initialize AppMessage inbox and outbox buffers with a suitable size
    const int inbox_size = 128;
    const int outbox_size = 128;
    app_message_open(inbox_size, outbox_size);

    // Configure the random text layer
    random_text_layer = text_layer_create(GRect(10,60, frame.size.w-20, frame.size.h-120));
    text_layer_set_font(random_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_background_color(random_text_layer, GColorWhite);
    text_layer_set_text_color(random_text_layer, GColorRed);
    text_layer_set_text_alignment(random_text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(random_text_layer));
  
    // Configure the city text layer
    city_text_layer = text_layer_create(GRect(10, 80, frame.size.w-20, frame.size.h-140));
    text_layer_set_font(city_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_background_color(city_text_layer, GColorWhite);
    text_layer_set_text_color(city_text_layer, GColorRed);
    text_layer_set_text_alignment(city_text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(city_text_layer));
    
    // Set the timers
    random_timer = app_timer_register(5000, ask_for_random, NULL);
    city_timer = app_timer_register(10000, ask_for_city, NULL);
}

static void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}