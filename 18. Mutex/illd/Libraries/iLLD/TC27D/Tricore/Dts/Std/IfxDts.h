/**
 * \file IfxDts.h
 * \brief DTS  basic functionality
 * \ingroup IfxLld_Dts
 *
 * \version iLLD_1_0_1_12_0
 * \copyright Copyright (c) 2017 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Dts_Std_Control Control functions
 * \ingroup IfxLld_Dts_Std
 * \defgroup IfxLld_Dts_Std_Status Status functions
 * \ingroup IfxLld_Dts_Std
 */

#ifndef IFXDTS_H
#define IFXDTS_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "illd\Libraries\iLLD\TC27D\Tricore\_Impl/IfxDts_cfg.h"
#include "illd\Libraries\Infra\Sfr\TC27D\_Reg\IfxScu_reg.h"
#include "illd\Libraries\Infra\Sfr\TC27D\_Reg\IfxScu_bf.h"
#include "illd\Libraries\iLLD\TC27D\Tricore\Cpu\Std/IfxCpu_Intrinsics.h"
#include "illd\Libraries\iLLD\TC27D\Tricore\Src\Std/IfxSrc.h"
/** \addtogroup IfxLld_Dts_Std_Control
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Disable the writing of register bits except START bit.
 * \return None
 */
IFX_INLINE void IfxDts_disableSensorControl(void);

/** \brief Enables the DTS sensor
 * \return None
 */
IFX_INLINE void IfxDts_enableSensor(void);

/** \brief Starts the next temperature measurement.
 * \return None
 */
IFX_INLINE void IfxDts_startSensor(void);

/** \} */

/** \addtogroup IfxLld_Dts_Std_Status
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Gets the Interrupt request source.
 * \return Address of interrupt request source
 */
IFX_INLINE volatile Ifx_SRC_SRCR *IfxDts_getSrcPointer(void);

/** \brief Read the temperature measurement result
 * \return Temperature measurement result
 */
IFX_INLINE uint16 IfxDts_getTemperatureValue(void);

/** \brief Returns the current BUSY status of the Sensor
 * \return TRUE if Sensor is busy in measuring temperature otherwise FALSE
 */
IFX_INLINE boolean IfxDts_isBusy(void);

/** \} */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the current READY status of the Sensor.
 * \return TRUE if Sensor is ready for measuring temperature otherwise FALSE
 */
IFX_EXTERN boolean IfxDts_isReady(void);

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxDts_disableSensorControl(void)
{
    MODULE_SCU.DTSCON.B.SLCK = 1;
    MODULE_SCU.DTSLIM.B.SLCK = 1;
}


IFX_INLINE void IfxDts_enableSensor(void)
{
    MODULE_SCU.DTSCON.B.PWD = 0;
}


IFX_INLINE volatile Ifx_SRC_SRCR *IfxDts_getSrcPointer(void)
{
    return &SRC_SCUDTS;
}


IFX_INLINE uint16 IfxDts_getTemperatureValue(void)
{
    return (uint16)MODULE_SCU.DTSSTAT.B.RESULT;
}


IFX_INLINE boolean IfxDts_isBusy(void)
{
    return MODULE_SCU.DTSSTAT.B.BUSY == 1 ? TRUE : FALSE;
}


IFX_INLINE void IfxDts_startSensor(void)
{
    MODULE_SCU.DTSCON.B.START = 1;
}


#endif /* IFXDTS_H */
