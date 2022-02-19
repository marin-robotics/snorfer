#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);

//define motors
pros::Motor front_left_mtr (8, MOTOR_GEARSET_06, true);
pros::Motor middle_left_mtr (9, MOTOR_GEARSET_06, true);
pros::Motor back_left_mtr (10, MOTOR_GEARSET_06, true);
pros::Motor front_right_mtr (5, MOTOR_GEARSET_06);
pros::Motor middle_right_mtr (6, MOTOR_GEARSET_06);
pros::Motor back_right_mtr (7, MOTOR_GEARSET_06);

pros::Motor front_arm (1, MOTOR_GEARSET_36, true);
pros::Motor back_arm (4, MOTOR_GEARSET_36);
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
  front_arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	//motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	back_arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
 //true is awpa false is awpb
bool auton_mode = true;

void autonomous() {

if (auton_mode == true) {
	front_left_mtr.move_absolute(800, 50);
	middle_left_mtr.move_absolute(800, 50);
	back_left_mtr.move_absolute(800, 50);
	front_right_mtr.move_absolute(800, 50);
	middle_right_mtr.move_absolute(800, 50);
	back_right_mtr.move_absolute(800, 50);
	pros::delay(1000);
	front_arm.tare_position();
	front_arm.move_absolute(1000, 30);
	pros::delay(1000);
	front_left_mtr.move_absolute(1000, 100);
	middle_left_mtr.move_absolute(1000, 100);
	back_left_mtr.move_absolute(1000, 100);
	front_right_mtr.move_absolute(1000, 100);
	middle_right_mtr.move_absolute(1000, 100);
	back_right_mtr.move_absolute(1000, 100);
}
if (auton_mode == false) {
	front_arm.tare_position();
	front_arm.move_absolute(1000, 30);
}

}
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	while (true) {
		if (master.get_digital_new_press(DIGITAL_A)) {
			auton_mode = false;
			master.print(0, 0, "Autonomous A");
		}
		if (master.get_digital_new_press(DIGITAL_B)) {
			auton_mode = true;
			master.print(0, 0, "Autonomous B");
		}

		pros::lcd::print(1, "                             %d", front_arm.is_over_temp());

		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right_y = master.get_analog(ANALOG_RIGHT_Y);
		int right_x = master.get_analog(ANALOG_RIGHT_X);
		float motor_mult = 1.5;
		//right
		front_right_mtr.move_velocity((right_y - right_x)*motor_mult);
		middle_right_mtr.move_velocity((right_y - right_x)*motor_mult);
		back_right_mtr.move_velocity((right_y - right_x)*motor_mult);
		//left
		front_left_mtr.move_velocity((right_y + right_x)*motor_mult);
		middle_left_mtr.move_velocity((right_y - right_x)*motor_mult);
		back_left_mtr.move_velocity((right_y + right_x)*motor_mult);


//test
		bool left_front_bumper = master.get_digital(DIGITAL_L1);
		bool left_back_bumper = master.get_digital(DIGITAL_L2);
		bool right_front_bumper = master.get_digital(DIGITAL_R1);
		bool right_back_bumper = master.get_digital(DIGITAL_R2);

		if (left_front_bumper) {
			back_arm = -127;
		} else if (left_back_bumper) {
			back_arm = 127;
		} else {
			back_arm = 0;
		}

		if (right_front_bumper) {
			front_arm = -127;
		} else if (right_back_bumper) {
			front_arm = 127;
		} else {
			front_arm = 0;
		}

		pros::delay(20);
	}
}
