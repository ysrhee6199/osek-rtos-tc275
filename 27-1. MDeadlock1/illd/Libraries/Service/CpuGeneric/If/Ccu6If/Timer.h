/**
 * \file Timer.h
 * \brief Timer interface
 *
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
 *
 * $Date: 2014-02-27 20:08:19 GMT$
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
 * \defgroup library_srvsw_if_timer Timer interface
 * \ingroup library_srvsw_if
 *
 */

#ifndef TIMER_H_
#define TIMER_H_ 1

#include "illd\Libraries\iLLD\TC27D\Tricore\Cpu\Std/Ifx_Types.h"

typedef struct Timer_s Timer;
typedef void         (*Timer_Start)(Timer *handle);
typedef void         (*Timer_Stop)(Timer *handle);
typedef void         (*Timer_SynchronousStart)(Timer *handle);
typedef void         (*Timer_SynchronousStop)(Timer *handle);
typedef void         (*Timer_CountOneStep)(Timer *handle);
typedef void         (*Timer_StartSingleShotMode)(Timer *handle);

typedef struct
{
    Timer_Start               start;
    Timer_Stop                stop;
    Timer_SynchronousStart    synchronousStart;
    Timer_SynchronousStop     synchronousStop;
    Timer_CountOneStep        countOneStep;
    Timer_StartSingleShotMode startSingleShotMode;
} Timer_Functions;

/** \brief Structure of the Timer interface */
struct Timer_s
{
    float32         t12Frequency;  /**< \brief Actual timer12 frequency */
    Ifx_TimerValue  t12Period;     /**< \brief Actual timer 12 period */
    float32         t13Frequency;  /**< \brief Actual timer13 frequency */
    Ifx_TimerValue  t13Period;     /**< \brief Actual timer 13 period */
#if IFX_CFG_USE_STANDARD_INTERFACE
    Timer_Functions functions;     /**< \brief  timer functions */
#endif
};

/** \brief Configuration structure of the Timer interface */
typedef struct
{
    float32        t12Frequency;   /**< \brief Specify expected T12 PWM frequency in Hertz */
    Ifx_TimerValue t12Period;      /**< \brief Specify expected T12 PWM period in ticks. */
    float32        t13Frequency;   /**< \brief Specify expected T13 PWM frequency in Hertz */
    Ifx_TimerValue t13Period;      /**< \brief Specify expected T13 PWM period in ticks. */
    Ifx_TimerValue waitingTime;    /**< \brief Specify the expected ticks before timer starts (TWAIT) */
    Ifx_TimerValue activeCount;    /**< \brief Specify active PWM period (TCOUNT) */
} Timer_Config;

#if IFX_CFG_USE_STANDARD_INTERFACE
IFX_INLINE void Timer_start(Timer *handle)               {handle->functions.start(handle); }
IFX_INLINE void Timer_stop(Timer *handle)                {handle->functions.stop(handle); }
IFX_INLINE void Timer_synchronousStart(Timer *handle)    {handle->functions.synchronousStart(handle); }
IFX_INLINE void Timer_synchronousStop(Timer *handle)     {handle->functions.synchronousStop(handle); }
IFX_INLINE void Timer_countOneStep(Timer *handle)        {handle->functions.countOneStep(handle); }
IFX_INLINE void Timer_startSingleShotMode(Timer *handle) {handle->functions.startSingleShotMode(handle); }
#endif

#endif /* TIMER_H_ */
