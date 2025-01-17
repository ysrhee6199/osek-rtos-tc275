/**
 * \file IfxGtm_Atom_Pwm.c
 * \brief GTM PWM details
 *
 * \version iLLD_1_0_1_12_0
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees. If and as long as no such terms
 * of use are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxGtm_Atom_Pwm.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

boolean IfxGtm_Atom_Pwm_init(IfxGtm_Atom_Pwm_Driver *driver, const IfxGtm_Atom_Pwm_Config *config)
{
    boolean result = TRUE;

    driver->gtm       = config->gtm;
    driver->atomIndex = config->atom;

    Ifx_GTM_ATOM     *atomSFR = &config->gtm->ATOM[config->atom];
    driver->atom        = atomSFR;
    driver->atomChannel = config->atomChannel;
    Ifx_GTM_ATOM_AGC *agc = &atomSFR->AGC;
    driver->agc         = agc;

    /* Initialize the timer part */
    if (config->synchronousUpdateEnabled == 1)
    {
        IfxGtm_Atom_Agc_enableChannelUpdate(agc, config->atomChannel, TRUE);
    }

    IfxGtm_Atom_Agc_setChannelForceUpdate(agc, config->atomChannel, TRUE, TRUE);

    IfxGtm_Atom_Ch_setSignalLevel(atomSFR, config->atomChannel, config->signalLevel);

    IfxGtm_Atom_Ch_setMode(atomSFR, config->atomChannel, config->mode);

    if (config->pin.outputPin != NULL_PTR)
    {
        IfxGtm_PinMap_setAtomTout(config->pin.outputPin, config->pin.outputMode, config->pin.padDriver);
    }

    // enable and initialise interrupts if chosen
    if ((config->interrupt.ccu0Enabled == 1) || (config->interrupt.ccu1Enabled == 1))
    {
        IfxGtm_Atom_Ch_setNotification(atomSFR, config->atomChannel, config->interrupt.mode, config->interrupt.ccu0Enabled, config->interrupt.ccu1Enabled);

        volatile Ifx_SRC_SRCR *src;
        src = IfxGtm_Atom_Ch_getSrcPointer(config->gtm, config->atom, config->atomChannel);
        IfxSrc_init(src, config->interrupt.isrProvider, config->interrupt.isrPriority);
        IfxSrc_enable(src);
    }

    if (config->synchronousUpdateEnabled == 1)
    {
        IfxGtm_Atom_Ch_setCompareZeroShadow(atomSFR, config->atomChannel, config->period);
        IfxGtm_Atom_Ch_setCompareOneShadow(atomSFR, config->atomChannel, config->dutyCycle);
    }
    else
    {
        IfxGtm_Atom_Ch_setCompareZero(atomSFR, config->atomChannel, config->period);
        IfxGtm_Atom_Ch_setCompareOne(atomSFR, config->atomChannel, config->dutyCycle);
    }

    IfxGtm_Atom_Agc_enableChannel(agc, config->atomChannel, TRUE, FALSE);
    IfxGtm_Atom_Agc_enableChannelOutput(agc, config->atomChannel, TRUE, FALSE);

    if (config->immediateStartEnabled == TRUE)
    {
        IfxGtm_Atom_Agc_trigger(driver->agc);
    }

    return result;
}


void IfxGtm_Atom_Pwm_initConfig(IfxGtm_Atom_Pwm_Config *config, Ifx_GTM *gtm)
{
    config->gtm                      = gtm;
    config->atom                     = IfxGtm_Atom_0;
    config->atomChannel              = IfxGtm_Atom_Ch_0;
    config->period                   = 20;
    config->dutyCycle                = 10;
    config->signalLevel              = Ifx_ActiveState_high;
    config->mode                     = IfxGtm_Atom_Mode_outputPwm;
    config->oneShotModeEnabled       = FALSE;
    config->synchronousUpdateEnabled = FALSE;
    config->immediateStartEnabled    = TRUE;
    config->interrupt.ccu0Enabled    = FALSE;
    config->interrupt.ccu1Enabled    = FALSE;
    config->interrupt.mode           = IfxGtm_IrqMode_pulseNotify;
    config->interrupt.isrProvider    = IfxSrc_Tos_cpu0;
    config->interrupt.isrPriority    = 0;
    config->pin.outputPin            = NULL_PTR;
    config->pin.outputMode           = IfxPort_OutputMode_pushPull;
    config->pin.padDriver            = IfxPort_PadDriver_cmosAutomotiveSpeed1;
}


void IfxGtm_Atom_Pwm_start(IfxGtm_Atom_Pwm_Driver *driver, boolean immediate)
{
    /* Enable channel if not enabled already */
    IfxGtm_Atom_Agc_enableChannel(driver->agc, driver->atomChannel, TRUE, immediate);
    IfxGtm_Atom_Agc_enableChannelOutput(driver->agc, driver->atomChannel, TRUE, immediate);

    /* Trigger the start now */
    IfxGtm_Atom_Agc_trigger(driver->agc);
}


void IfxGtm_Atom_Pwm_stop(IfxGtm_Atom_Pwm_Driver *driver, boolean immediate)
{
    /* Disable channels */
    IfxGtm_Atom_Agc_enableChannel(driver->agc, driver->atomChannel, FALSE, immediate);
    IfxGtm_Atom_Agc_enableChannelOutput(driver->agc, driver->atomChannel, FALSE, immediate);

    /* Trigger the stop now */
    IfxGtm_Atom_Agc_trigger(driver->agc);
}
