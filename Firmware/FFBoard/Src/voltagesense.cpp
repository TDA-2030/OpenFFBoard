/*
 * voltagesense.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: Yannick
 */

#include "global_callbacks.h"
#include "constants.h"
#include "voltagesense.h"
#include "ErrorHandler.h"

bool braking_flag = false;
uint32_t maxVoltage = 65000; // Force braking
uint32_t voltageDiffActivate = 5000;
uint32_t voltageDiffDeactivate = 4000;
float vSenseMult = VOLTAGE_MULT_DEFAULT;
bool brake_failure = false;

uint32_t brakeActiveTime = 0;

/*
 * Multiplier to convert ADC counts to a sensed voltage on the vInt and vExt pins.
 */
void setVSenseMult(float vSenseMultiplier){
	vSenseMult = vSenseMultiplier;
}

/*
 * vMax: maximum voltage where the brake pin goes always high
 * vdiffAct: difference between vInt and vExt to activate brake pin
 * vdiffDeact: difference when to deactivate brake pin again (must be lower than vdiffAct)
 * Set vMax = 0 to completely deactivate the brake resistor function. DANGEROUS
 *
 */
void setupBrakePin(uint32_t vdiffAct,uint32_t vdiffDeact,uint32_t vMax){
	maxVoltage = vMax;
	voltageDiffActivate = vdiffAct;
	voltageDiffDeactivate = vdiffDeact;
}

uint16_t getIntV(){
#ifdef HW_ESP32SX
	return 20000;  // ESP32S2 don't have enough gpio to measure v_int
#else
	return VSENSE_ADC_BUF[ADC_CHAN_VINT] * vSenseMult;
#endif
}

uint16_t getExtV(){
#ifdef HW_ESP32SX
	return 20000; // ESP32S2 don't have enough gpio to measure v_ext
#else
	return VSENSE_ADC_BUF[ADC_CHAN_VEXT] * vSenseMult;
#endif
}

void brakeCheck(){
	if(maxVoltage == 0 || brake_failure){
		return;
	}
	uint16_t vint = getIntV();
	uint16_t vext = getExtV();
	bool lastBrakingFlag = braking_flag;
	braking_flag = (vint > vext + voltageDiffActivate) || vint > maxVoltage || (braking_flag && (vint > vext + voltageDiffDeactivate));
			//(ADC_BUF[ADC_CHAN_VINT] > ADC_BUF[ADC_CHAN_VEXT]+400 || (ADC_BUF[ADC_CHAN_VINT] > 3000));
	if(braking_flag != lastBrakingFlag){
		if(braking_flag){
			brakeActiveTime = HAL_GetTick();
		}else{
			brakeActiveTime = 0;
		}
	}
	if(brakeActiveTime && HAL_GetTick() - brakeActiveTime > 5000){
		// Brake resistor active over 5s. Shut down.
		brake_failure = true;
		HAL_GPIO_WritePin(DRV_BRAKE_GPIO_Port,DRV_BRAKE_Pin, GPIO_PIN_RESET);
		ErrorHandler::addError(Error(ErrorCode::brakeResistorFailure, ErrorType::critical, "Brake resistor stuck on"));
	}else{
		HAL_GPIO_WritePin(DRV_BRAKE_GPIO_Port,DRV_BRAKE_Pin, braking_flag ? GPIO_PIN_SET:GPIO_PIN_RESET);
	}

}
