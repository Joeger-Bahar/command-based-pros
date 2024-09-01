#pragma once

#include "command/subsystem.h"
#include "command/runCommand.h"

/**
 * Intake subsystem
 *
 * Allows controlling the percentage power of the intake subsystem
 */
class Intake : public Subsystem {
private:
	pros::Motor intakeMotor;
public:
	/**
	 * Construct a new Intake subsystem with pro::Motor object
	 *
	 * @param intake_motor
	 */
	explicit Intake(pros::Motor intake_motor)
		: intakeMotor(std::move(intake_motor)) {
	}

	/**
	 * @brief Periodic runs every frame (10ms) by the command scheduler after the subsystem is registered.
	 *
	 * @note Periodic is very useful tasks such as PID that need to run every frame, but the target is only set once.
	 */
	void periodic() override {
		// EX: debugging tasks
		std::cout << "Intake speed: " << intakeMotor.get_actual_velocity() << std::endl;

		// Also:
		// Updating PID for something like a flywheel or odometry for a drivetrain subsystem
	}

	// Create member functions to actuate the subsystem
	// EX: move the intake at a certain (signed) percentage of voltage.
	/**
	 * @brief This command moves the intake at a certain voltage potential requested as a percentage [-1, 1]
	 *
	 * @param pct Percent voltage for the intake in the range [-1, 1]
	 */
	void setPct(const double pct) {

		// Scale [-1, 1] to [-12000 mV, 12000 mV]
		this->intakeMotor.move_voltage(static_cast<std::int32_t>(pct * 12000.0));
	}

	// Example way to create commands from class
	// This is very useful for simple commands or default commands
	/**
	 *	@brief
	 *
	 * @param pct Percentage Desired for this command
	 * @return Command pointer to a RunCommand that moves the intake
	 */
	RunCommand* pctCommand(const double pct) {
		// Create a new RunCommand
		// The lambda body is called at every update, in this case setting the intake percentage
		return new RunCommand(
			[this, pct] () // Capture "this" and the percentage request
			{
				this->setPct(pct); // Set the percentage of the intake to the request
			},
			{this} // Add "this", the pointer to this subsystem that is currently running.
			       // It is important to ensure that all subsystems that are being utilized in a command are properly
			       // Freed to allow that command to run.
			);
	}

	// Free any additional resources that are needed.
	~Intake() override = default;
};