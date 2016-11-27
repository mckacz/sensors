/*
* Based on https://github.com/Yveaux/arduino_vcc (LGPL v2.1)
*/

#ifndef __VCC_H
#define __VCC_H

#include "Arduino.h"

class Vcc
{
  public:
    /**
     * Constructor
     *
     * @param correction  Correction factor, when reported Vcc is off from measured (externally) Vcc
     *                    (due to variations in bandgap voltage of +/- 0.1V)
     *                    Calculate as Vcc_measured/Vcc_reported. Defaults to 1.
     */
    Vcc( const float correction = 1.0 );

    /**
     * Retrieve current Vcc level.
     *
     * @return Current Vcc level, in Volts.
     */
    float Read_Volts(void);

    /**
     * Retrieve current 1.1V reference against AVcc
     */
    uint16_t Read_ADC(void);

    /**
     * Retrieve current Vcc level. The total voltage range shall be passed
     * as low/high bound. For e.g. an alkaline AA battery this range can be set
     * to [0.6,..,1.5] Volts.
     *
     * @param range_min  Low bound to Vcc level range, in Volts.
     * @param range_max  High bound to Vcc level range, in Volts.
     * @param clip       When set, assures returned percentage is clipped to [0..100]% range.
     * @return Current Vcc level, as percentage of expected Vcc level.
     */
    float Read_Perc(const float range_min = 0.0, const float range_max = 0.0, const boolean clip = true);

  protected:
    float m_correction;     /**< Correction factor, when reported Vcc is off. */ 
};

#endif 