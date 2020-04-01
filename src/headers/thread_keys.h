#pragma once

typedef enum keyboard_keys
{
    kb_null,
    kb_ctrl_a,
    kb_ctrl_b,
    kb_ctrl_c,
    kb_ctrl_d,
    kb_ctrl_e,
    kb_ctrl_f,
    kb_ctrl_g,
    
    kb_data_link_escape,

    kb_ctrl_i,
    kb_ctrl_j,
    kb_ctrl_k,
    kb_ctrl_l,
    kb_ctrl_m,
    kb_ctrl_n,
    kb_ctrl_o,
    kb_ctrl_p,
    kb_ctrl_q,
    kb_ctrl_r,
    kb_ctrl_s,
    kb_ctrl_t,
    kb_ctrl_u,
    kb_ctrl_v,
    kb_ctrl_w,
    kb_ctrl_x,
    kb_ctrl_y,
    kb_ctrl_z,
    kb_esc,

    kb_file_separator,
    kb_group_separator,
    kb_record_separator,
    kb_unit_separator,

    kb_space,
    kb_exclamation,
    kb_double_quotes,
    kb_hashtag,
    kb_dolar_sign,
    kb_percentage,
    kb_commercial_e,
    kb_single_quote,
    kb_opening_parenthesis,
    kb_closing_parenthesis,
    kb_asterisk,
    kb_plus_sign,
    kb_comma,
    kb_minus,
    kb_dot,
    kb_forward_slash,

    kb_number_0,
    kb_number_1,
    kb_number_2,
    kb_number_3,
    kb_number_4,
    kb_number_5,
    kb_number_6,
    kb_number_7,
    kb_number_8,
    kb_number_9,

    kb_double_dots,
    kb_dot_comma,
    kb_less_than,
    kb_equals,
    kb_more_than,
    kb_question_mark,
    kb_at_sign,

    kb_letter_A,
    kb_letter_B,
    kb_letter_C,
    kb_letter_D,
    kb_letter_E,
    kb_letter_F,
    kb_letter_G,
    kb_letter_H,
    kb_letter_I,
    kb_letter_J,
    kb_letter_K,
    kb_letter_L,
    kb_letter_M,
    kb_letter_N,
    kb_letter_O,
    kb_letter_P,
    kb_letter_Q,
    kb_letter_R,
    kb_letter_S,
    kb_letter_T,
    kb_letter_U,
    kb_letter_V,
    kb_letter_W,
    kb_letter_X,
    kb_letter_Y,
    kb_letter_Z,

    kb_opening_bracket,
    kb_backward_slash,
    kb_closing_bracket,
    kb_circumflex,
    kb_underscore,
    kb_acute,

    kb_letter_a,
    kb_letter_b,
    kb_letter_c,
    kb_letter_d,
    kb_letter_e,
    kb_letter_f,
    kb_letter_g,
    kb_letter_h,
    kb_letter_i,
    kb_letter_j,
    kb_letter_k,
    kb_letter_l,
    kb_letter_m,
    kb_letter_n,
    kb_letter_o,
    kb_letter_p,
    kb_letter_q,
    kb_letter_r,
    kb_letter_s,
    kb_letter_t,
    kb_letter_u,
    kb_letter_v,
    kb_letter_w,
    kb_letter_x,
    kb_letter_y,
    kb_letter_z,

    kb_opening_curly_braces,
    kb_straight,
    kb_closing_curly_braces,
    kb_tilda,


#ifdef _WIN32
    kb_ctrl_h,
#else
    kb_ctrl_h = 8,
#endif

#ifdef _WIN32
    kb_up_arrow = 61512,
    kb_left_arrow = 61515,
    kb_right_arrow = 61517,
    kb_bottom_arrow = 61520,

    kb_return = 13,

    kb_home = 61511,
    kb_page_up = 61513,
    kb_end = 61519,
    kb_page_down = 61521,

    kb_insert = 61522,
    kb_del = 61523,

    kb_f1 = 61499,
    kb_f2,
    kb_f3,
    kb_f4,
    kb_f5,
    kb_f6,
    kb_f7,
    kb_f8,
    kb_f9,
    kb_f10,
    kb_f12 = 57478,
#else
    kb_up_arrow = 1792833,
    kb_left_arrow = 1792836,
    kb_right_arrow = 1792835,
    kb_bottom_arrow = 1792834,

    kb_return = 10,

    kb_home = 1792840,
    kb_page_up = 458962302,
    kb_end = 1792838,
    kb_page_down = 458962558,

    kb_insert = 458961534,
    kb_del = 458961790,

    kb_f1 = 1789776,
    kb_f2,
    kb_f3,
    kb_f4,
    kb_f5 = 1529951614,
    kb_f6 = 1529952126,
    kb_f7 = 1529952382,
    kb_f8 = 1529952638,
    kb_f9 = 1530015870,
    kb_f10 = 1539916126,
    kb_f12 = 1530016894,
#endif

} kbd_keys_t;