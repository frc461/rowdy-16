#define SPEEDCONTROLCLASS Talon
#define DEADZONE 0.06

enum joysticks {
	js_0 = 0, //js = joy stick
	js_1 = 1,
	cs_a = 2, //cs = control stick it's the word for the button-y fake joysticks
	cs_b = 3
};

enum teh_talons {
	t_fl = 5, //t = talon, f = front, l = left, c = center, r = right, b = back
	t_fc = 0,
	t_fr = 2,
	t_bl = 4,
	t_bc = 3,
	t_br = 1
};

enum cs_a_buttons {
};

enum cs_b_buttons{
};
