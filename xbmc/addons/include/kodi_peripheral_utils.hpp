/*
 *      Copyright (C) 2014-2015 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "kodi_peripheral_types.h"

#include <cstring>
#include <map>
#include <string>
#include <vector>

#define PERIPHERAL_SAFE_DELETE(x)        do { delete   (x); (x) = NULL; } while (0)
#define PERIPHERAL_SAFE_DELETE_ARRAY(x)  do { delete[] (x); (x) = NULL; } while (0)

namespace ADDON
{
  /*!
   * Utility class to manipulate arrays of peripheral types.
   */
  template <class THE_CLASS, typename THE_STRUCT>
  class PeripheralVector
  {
  public:
    static void ToStructs(const std::vector<THE_CLASS>& vecObjects, THE_STRUCT** pStructs)
    {
      if (!pStructs)
        return;

      if (vecObjects.empty())
      {
        *pStructs = NULL;
      }
      else
      {
        (*pStructs) = new THE_STRUCT[vecObjects.size()];
        for (unsigned int i = 0; i < vecObjects.size(); i++)
          vecObjects.at(i).ToStruct((*pStructs)[i]);
      }
    }

    static void ToStructs(const std::vector<THE_CLASS*>& vecObjects, THE_STRUCT** pStructs)
    {
      if (!pStructs)
        return;

      if (vecObjects.empty())
      {
        *pStructs = NULL;
      }
      else
      {
        *pStructs = new THE_STRUCT[vecObjects.size()];
        for (unsigned int i = 0; i < vecObjects.size(); i++)
          vecObjects.at(i)->ToStruct((*pStructs)[i]);
      }
    }

    static void FreeStructs(unsigned int structCount, THE_STRUCT* structs)
    {
      if (structs)
      {
        for (unsigned int i = 0; i < structCount; i++)
          THE_CLASS::FreeStruct(structs[i]);
      }
      PERIPHERAL_SAFE_DELETE_ARRAY(structs);
    }
  };

  /*!
   * ADDON::Peripheral
   *
   * Wrapper class providing peripheral information. Classes can extend
   * Peripheral to inherit peripheral properties.
   */
  class Peripheral
  {
  public:
    Peripheral(PERIPHERAL_TYPE type = PERIPHERAL_TYPE_UNKNOWN, const std::string& strName = "") :
      m_type(type),
      m_strName(strName),
      m_vendorId(0),
      m_productId(0),
      m_index(0)
    {
    }

    Peripheral(const PERIPHERAL_INFO& info) :
      m_type(info.type),
      m_strName(info.name ? info.name : ""),
      m_vendorId(info.vendor_id),
      m_productId(info.product_id),
      m_index(info.index)
    {
    }

    virtual ~Peripheral(void) { }

    PERIPHERAL_TYPE    Type(void) const      { return m_type; }
    const std::string& Name(void) const      { return m_strName; }
    uint16_t           VendorID(void) const  { return m_vendorId; }
    uint16_t           ProductID(void) const { return m_productId; }
    unsigned int       Index(void) const     { return m_index; }

    void SetType(PERIPHERAL_TYPE type)       { m_type      = type; }
    void SetName(const std::string& strName) { m_strName   = strName; }
    void SetVendorID(uint16_t vendorId)      { m_vendorId  = vendorId; }
    void SetProductID(uint16_t productId)    { m_productId = productId; }
    void SetIndex(unsigned int index)        { m_index     = index; }

    void ToStruct(PERIPHERAL_INFO& info) const
    {
      info.type       = m_type;
      info.name       = new char[m_strName.size() + 1];
      info.vendor_id  = m_vendorId;
      info.product_id = m_productId;
      info.index      = m_index;

      std::strcpy(info.name, m_strName.c_str());
    }

    static void FreeStruct(PERIPHERAL_INFO& info)
    {
      PERIPHERAL_SAFE_DELETE_ARRAY(info.name);
    }

  private:
    PERIPHERAL_TYPE  m_type;
    std::string      m_strName;
    uint16_t         m_vendorId;
    uint16_t         m_productId;
    unsigned int     m_index;
  };

  typedef PeripheralVector<Peripheral, PERIPHERAL_INFO> Peripherals;

  /*!
   * ADDON::PeripheralEvent
   *
   * Wrapper class for peripheral events.
   */
  class PeripheralEvent
  {
  public:
    PeripheralEvent(void) :
      m_event()
    {
    }

    PeripheralEvent(unsigned int peripheralIndex, unsigned int buttonIndex, JOYSTICK_STATE_BUTTON state) :
      m_event()
    {
      SetType(PERIPHERAL_EVENT_TYPE_DRIVER_BUTTON);
      SetPeripheralIndex(peripheralIndex);
      SetDriverIndex(buttonIndex);
      SetButtonState(state);
    }

    PeripheralEvent(unsigned int peripheralIndex, unsigned int hatIndex, JOYSTICK_STATE_HAT state) :
      m_event()
    {
      SetType(PERIPHERAL_EVENT_TYPE_DRIVER_HAT);
      SetPeripheralIndex(peripheralIndex);
      SetDriverIndex(hatIndex);
      SetHatState(state);
    }

    PeripheralEvent(unsigned int peripheralIndex, unsigned int axisIndex, JOYSTICK_STATE_AXIS state) :
      m_event()
    {
      SetType(PERIPHERAL_EVENT_TYPE_DRIVER_AXIS);
      SetPeripheralIndex(peripheralIndex);
      SetDriverIndex(axisIndex);
      SetAxisState(state);
    }

    PeripheralEvent(const PERIPHERAL_EVENT& event) :
      m_event(event)
    {
    }

    PERIPHERAL_EVENT_TYPE Type(void) const            { return m_event.type; }
    unsigned int          PeripheralIndex(void) const { return m_event.peripheral_index; }
    unsigned int          DriverIndex(void) const     { return m_event.driver_index; }
    JOYSTICK_STATE_BUTTON ButtonState(void) const     { return m_event.driver_button_state; }
    JOYSTICK_STATE_HAT    HatState(void) const        { return m_event.driver_hat_state; }
    JOYSTICK_STATE_AXIS   AxisState(void) const       { return m_event.driver_axis_state; }

    void SetType(PERIPHERAL_EVENT_TYPE type)         { m_event.type                = type; }
    void SetPeripheralIndex(unsigned int index)      { m_event.peripheral_index    = index; }
    void SetDriverIndex(unsigned int index)          { m_event.driver_index        = index; }
    void SetButtonState(JOYSTICK_STATE_BUTTON state) { m_event.driver_button_state = state; }
    void SetHatState(JOYSTICK_STATE_HAT state)       { m_event.driver_hat_state    = state; }
    void SetAxisState(JOYSTICK_STATE_AXIS state)     { m_event.driver_axis_state   = state; }

    void ToStruct(PERIPHERAL_EVENT& event) const
    {
      event = m_event;
    }

    static void FreeStruct(PERIPHERAL_EVENT& event)
    {
      (void)event;
    }

  private:
    PERIPHERAL_EVENT m_event;
  };

  typedef PeripheralVector<PeripheralEvent, PERIPHERAL_EVENT> PeripheralEvents;

  /*!
   * ADDON::Joystick
   *
   * Wrapper class providing additional joystick information not provided by
   * ADDON::Peripheral.
   */
  class Joystick : public Peripheral
  {
  public:
    Joystick(const std::string& provider = "", const std::string& strName = "") :
      Peripheral(PERIPHERAL_TYPE_JOYSTICK, strName),
      m_provider(provider),
      m_requestedPort(NO_PORT_REQUESTED),
      m_buttonCount(0),
      m_hatCount(0),
      m_axisCount(0)
    {
    }

    Joystick(const Joystick& other)
    {
      *this = other;
    }

    Joystick(const JOYSTICK_INFO& info) :
      Peripheral(info.peripheral),
      m_provider(info.provider ? info.provider : ""),
      m_requestedPort(info.requested_port),
      m_buttonCount(info.button_count),
      m_hatCount(info.hat_count),
      m_axisCount(info.axis_count)
    {
    }

    virtual ~Joystick(void) { }

    Joystick& operator=(const Joystick& rhs)
    {
      if (this != &rhs)
      {
        Peripheral::operator=(rhs);

        m_provider      = rhs.m_provider;
        m_requestedPort = rhs.m_requestedPort;
        m_buttonCount   = rhs.m_buttonCount;
        m_hatCount      = rhs.m_hatCount;
        m_axisCount     = rhs.m_axisCount;
      }
      return *this;
    }

    const std::string& Provider(void) const      { return m_provider; }
    int                RequestedPort(void) const { return m_requestedPort; }
    unsigned int       ButtonCount(void) const   { return m_buttonCount; }
    unsigned int       HatCount(void) const      { return m_hatCount; }
    unsigned int       AxisCount(void) const     { return m_axisCount; }

    void SetProvider(const std::string& provider)     { m_provider      = provider; }
    void SetRequestedPort(int requestedPort)          { m_requestedPort = requestedPort; }
    void SetButtonCount(unsigned int buttonCount)     { m_buttonCount   = buttonCount; }
    void SetHatCount(unsigned int hatCount)           { m_hatCount      = hatCount; }
    void SetAxisCount(unsigned int axisCount)         { m_axisCount     = axisCount; }

    void ToStruct(JOYSTICK_INFO& info) const
    {
      Peripheral::ToStruct(info.peripheral);

      info.provider       = new char[m_provider.size() + 1];
      info.requested_port = m_requestedPort;
      info.button_count   = m_buttonCount;
      info.hat_count      = m_hatCount;
      info.axis_count     = m_axisCount;

      std::strcpy(info.provider, m_provider.c_str());
    }

    static void FreeStruct(JOYSTICK_INFO& info)
    {
      Peripheral::FreeStruct(info.peripheral);

      PERIPHERAL_SAFE_DELETE_ARRAY(info.provider);
    }

  private:
    std::string                   m_provider;
    int                           m_requestedPort;
    unsigned int                  m_buttonCount;
    unsigned int                  m_hatCount;
    unsigned int                  m_axisCount;
  };

  typedef PeripheralVector<Joystick, JOYSTICK_INFO> Joysticks;

  /*!
   * ADDON::JoystickFeature
   *
   * Base class for joystick features. In kodi_peripheral_types.h, the various
   * driver types are stored as an union. Here, we use polymorphism to allow for
   * extra driver information.
   */
  class JoystickFeature
  {
  public:
    JoystickFeature(void)
    {
    }

    JoystickFeature(const std::string& name) :
      m_name(name)
    {
    }

    JoystickFeature(const JOYSTICK_FEATURE& feature) :
      m_name(feature.feature_name ? feature.feature_name : "")
    {
    }

    virtual ~JoystickFeature(void) { }

    virtual JoystickFeature* Clone(void) const { return new JoystickFeature(*this); }

    virtual bool Equals(const JoystickFeature* other) const
    {
      return other &&
             m_name == other->m_name &&
             Type() == other->Type();
    }

    virtual JOYSTICK_DRIVER_TYPE Type(void) const { return JOYSTICK_DRIVER_TYPE_UNKNOWN; }
    const std::string&           Name(void) const { return m_name; }

    void SetName(const std::string& name) { m_name = name; }

    virtual void ToStruct(JOYSTICK_FEATURE& feature) const
    {
      feature.feature_name = new char[m_name.length() + 1];
      feature.driver_type  = Type();

      std::strcpy(feature.feature_name, m_name.c_str());
    }

    static void FreeStruct(JOYSTICK_FEATURE& feature)
    {
      PERIPHERAL_SAFE_DELETE_ARRAY(feature.feature_name);
    }

  private:
    std::string  m_name;
  };

  typedef PeripheralVector<JoystickFeature, JOYSTICK_FEATURE> JoystickFeatures;

  /*!
   * ADDON::DriverButton
   *
   * Joystick feature is a button that can be read from the driver.
   */
  class DriverButton : public JoystickFeature
  {
  public:
    DriverButton(void) :
      m_index(DRIVER_INDEX_UNKNOWN)
    {
    }

    DriverButton(const std::string& name, int index) :
      JoystickFeature(name),
      m_index(index)
    {
    }

    DriverButton(const JOYSTICK_FEATURE& feature) :
      JoystickFeature(feature),
      m_index(feature.driver_button.index)
    {
    }

    virtual ~DriverButton(void) { }

    virtual JoystickFeature* Clone(void) const { return new DriverButton(*this); }

    virtual bool Equals(const JoystickFeature* other) const
    {
      return JoystickFeature::Equals(other) &&
             m_index == static_cast<const DriverButton*>(other)->m_index;
    }

    virtual JOYSTICK_DRIVER_TYPE Type(void) const  { return JOYSTICK_DRIVER_TYPE_BUTTON; }
    int                          Index(void) const { return m_index; }

    void SetIndex(int index) { m_index = index; }

    virtual void ToStruct(JOYSTICK_FEATURE& feature) const
    {
      JoystickFeature::ToStruct(feature);
      feature.driver_button.index = m_index;
    }

  private:
    int m_index;
  };

  /*!
   * ADDON::DriverHat
   *
   * Joystick feature is a cardinal direction of the driver's hat
   */
  class DriverHat : public JoystickFeature
  {
  public:
    DriverHat(void) :
      m_index(DRIVER_INDEX_UNKNOWN),
      m_direction(JOYSTICK_DRIVER_HAT_UNKNOWN)
    {
    }

    DriverHat(const std::string& name, int index, JOYSTICK_DRIVER_HAT_DIRECTION direction) :
      JoystickFeature(name),
      m_index(index),
      m_direction(direction)
    {
    }

    DriverHat(const JOYSTICK_FEATURE& feature) :
      JoystickFeature(feature),
      m_index(feature.driver_hat.index),
      m_direction(feature.driver_hat.direction)
    {
    }

    virtual ~DriverHat(void) { }

    virtual JoystickFeature* Clone(void) const { return new DriverHat(*this); }

    virtual bool Equals(const JoystickFeature* other) const
    {
      return JoystickFeature::Equals(other) &&
             m_index     == static_cast<const DriverHat*>(other)->m_index &&
             m_direction == static_cast<const DriverHat*>(other)->m_direction;
    }

    virtual JOYSTICK_DRIVER_TYPE  Type(void) const      { return JOYSTICK_DRIVER_TYPE_HAT_DIRECTION; }
    int                           Index(void) const     { return m_index; }
    JOYSTICK_DRIVER_HAT_DIRECTION Direction(void) const { return m_direction; }

    void SetIndex(int index)                                   { m_index     = index; }
    void SetDirection(JOYSTICK_DRIVER_HAT_DIRECTION direction) { m_direction = direction; }

    virtual void ToStruct(JOYSTICK_FEATURE& feature) const
    {
      JoystickFeature::ToStruct(feature);
      feature.driver_hat.index     = m_index;
      feature.driver_hat.direction = m_direction;
    }

  private:
    int                           m_index;
    JOYSTICK_DRIVER_HAT_DIRECTION m_direction;
  };

  /*!
   * ADDON::DriverSemiAxis
   *
   * Joystick feature is the positive or negative half of an axis in the driver.
   */
  class DriverSemiAxis : public JoystickFeature
  {
  public:
    DriverSemiAxis(void) :
      m_index(DRIVER_INDEX_UNKNOWN),
      m_direction(JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_UNKNOWN)
    {
    }

    DriverSemiAxis(const std::string& name, int index, JOYSTICK_DRIVER_SEMIAXIS_DIRECTION direction) :
      JoystickFeature(name),
      m_index(index),
      m_direction(direction)
    {
    }

    DriverSemiAxis(const JOYSTICK_FEATURE& feature) :
      JoystickFeature(feature),
      m_index(feature.driver_semiaxis.index),
      m_direction(feature.driver_semiaxis.direction)
    {
    }

    virtual ~DriverSemiAxis(void) { }

    virtual JoystickFeature* Clone(void) const { return new DriverSemiAxis(*this); }

    virtual bool Equals(const JoystickFeature* other) const
    {
      return JoystickFeature::Equals(other) &&
             m_index     == static_cast<const DriverSemiAxis*>(other)->m_index &&
             m_direction == static_cast<const DriverSemiAxis*>(other)->m_direction;
    }

    virtual JOYSTICK_DRIVER_TYPE       Type(void) const      { return JOYSTICK_DRIVER_TYPE_SEMIAXIS; }
    int                                Index(void) const     { return m_index; }
    JOYSTICK_DRIVER_SEMIAXIS_DIRECTION Direction(void) const { return m_direction; }

    void SetIndex(int index)                                        { m_index     = index; }
    void SetDirection(JOYSTICK_DRIVER_SEMIAXIS_DIRECTION direction) { m_direction = direction; }

    virtual void ToStruct(JOYSTICK_FEATURE& feature) const
    {
      JoystickFeature::ToStruct(feature);
      feature.driver_semiaxis.index     = m_index;
      feature.driver_semiaxis.direction = m_direction;
    }

  private:
    int                                m_index;
    JOYSTICK_DRIVER_SEMIAXIS_DIRECTION m_direction;
  };

  /*!
   * ADDON::DriverAnalogStick
   *
   * Joystick feature is an analog stick associated with two axes in the driver.
   */
  class DriverAnalogStick : public JoystickFeature
  {
  public:
    DriverAnalogStick(void) :
      m_up(new JoystickFeature),
      m_down(new JoystickFeature),
      m_right(new JoystickFeature),
      m_left(new JoystickFeature)
    {
    }

    DriverAnalogStick(const std::string& name, int xIndex, bool xInverted, int yIndex, bool yInverted) :
      JoystickFeature(name),
      m_up(new DriverSemiAxis("",    xIndex, xInverted ? JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_NEGATIVE : JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_POSITIVE)),
      m_down(new DriverSemiAxis("",  xIndex, xInverted ? JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_POSITIVE : JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_NEGATIVE)),
      m_right(new DriverSemiAxis("", yIndex, yInverted ? JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_NEGATIVE : JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_POSITIVE)),
      m_left(new DriverSemiAxis("",  yIndex, yInverted ? JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_POSITIVE : JOYSTICK_DRIVER_SEMIAXIS_DIRECTION_NEGATIVE))
    {
    }

    DriverAnalogStick(const JOYSTICK_FEATURE& feature) :
      JoystickFeature(feature),
      m_up(MakeFeature(feature.driver_analog_stick.up)),
      m_down(MakeFeature(feature.driver_analog_stick.down)),
      m_right(MakeFeature(feature.driver_analog_stick.right)),
      m_left(MakeFeature(feature.driver_analog_stick.left))
    {
    }

    virtual ~DriverAnalogStick(void)
    {
      delete m_up;
      delete m_down;
      delete m_right;
      delete m_left;
    }

    virtual JoystickFeature* Clone(void) const { return new DriverAnalogStick(*this); }

    virtual bool Equals(const JoystickFeature* other) const
    {
      return JoystickFeature::Equals(other) &&
             m_up->Equals(static_cast<const DriverAnalogStick*>(other)->m_up)       &&
             m_down->Equals(static_cast<const DriverAnalogStick*>(other)->m_down)   &&
             m_right->Equals(static_cast<const DriverAnalogStick*>(other)->m_right) &&
             m_left->Equals(static_cast<const DriverAnalogStick*>(other)->m_left);
    }

    virtual JOYSTICK_DRIVER_TYPE Type(void) const  { return JOYSTICK_DRIVER_TYPE_ANALOG_STICK; }
    const JoystickFeature*       Up(void) const    { return m_up; }
    const JoystickFeature*       Down(void) const  { return m_down; }
    const JoystickFeature*       Right(void) const { return m_right; }
    const JoystickFeature*       Left(void) const  { return m_left; }

    void SetUp(const JoystickFeature* feature)    { delete m_up;    m_up    = feature->Clone(); }
    void SetDown(const JoystickFeature* feature)  { delete m_down;  m_down  = feature->Clone(); }
    void SetRight(const JoystickFeature* feature) { delete m_right; m_right = feature->Clone(); }
    void SetLeft(const JoystickFeature* feature)  { delete m_left;  m_left  = feature->Clone(); }

    virtual void ToStruct(JOYSTICK_FEATURE& feature) const
    {
      JoystickFeature::ToStruct(feature);
      ToDirectionStruct(m_up,    feature.driver_analog_stick.up);
      ToDirectionStruct(m_down,  feature.driver_analog_stick.down);
      ToDirectionStruct(m_right, feature.driver_analog_stick.right);
      ToDirectionStruct(m_left,  feature.driver_analog_stick.left);
    }

  private:
    JoystickFeature* MakeFeature(const JOYSTICK_DRIVER_ANALOG_STICK_DIRECTION& analog_stick_dir)
    {
      switch (analog_stick_dir.type)
      {
      case JOYSTICK_DRIVER_TYPE_BUTTON:
        return new DriverButton("", analog_stick_dir.button.index);
      case JOYSTICK_DRIVER_TYPE_HAT_DIRECTION:
        return new DriverHat("", analog_stick_dir.hat.index, analog_stick_dir.hat.direction);
      case JOYSTICK_DRIVER_TYPE_SEMIAXIS:
        return new DriverSemiAxis("", analog_stick_dir.semiaxis.index, analog_stick_dir.semiaxis.direction);
      default:
        return new JoystickFeature;
      }
    }

    void ToDirectionStruct(const JoystickFeature* feature, JOYSTICK_DRIVER_ANALOG_STICK_DIRECTION& analog_stick_dir) const
    {
      analog_stick_dir.type = feature->Type();
      if (feature->Type() != JOYSTICK_DRIVER_TYPE_UNKNOWN)
      {
        JOYSTICK_FEATURE featureStruct;
        feature->ToStruct(featureStruct);

        switch (feature->Type())
        {
        case JOYSTICK_DRIVER_TYPE_BUTTON:
          analog_stick_dir.button = featureStruct.driver_button;
          break;
        case JOYSTICK_DRIVER_TYPE_HAT_DIRECTION:
          analog_stick_dir.hat = featureStruct.driver_hat;
          break;
        case JOYSTICK_DRIVER_TYPE_SEMIAXIS:
          analog_stick_dir.semiaxis = featureStruct.driver_semiaxis;
          break;
        default:
          break;
        }

        JoystickFeature::FreeStruct(featureStruct);
      }
    }

    JoystickFeature* m_up;
    JoystickFeature* m_down;
    JoystickFeature* m_right;
    JoystickFeature* m_left;
  };

  /*!
   * ADDON::DriverAnalogStick
   *
   * Joystick feature is an accelerometer associated with three axes in the
   * driver.
   */
  class DriverAccelerometer : public JoystickFeature
  {
  public:
    DriverAccelerometer(void) :
      m_xIndex(DRIVER_INDEX_UNKNOWN),
      m_xInverted(false),
      m_yIndex(DRIVER_INDEX_UNKNOWN),
      m_yInverted(false),
      m_zIndex(DRIVER_INDEX_UNKNOWN),
      m_zInverted(false)
    {
    }

    DriverAccelerometer(const std::string& name, int xIndex, bool xInverted, int yIndex, bool yInverted, int zIndex, bool zInverted) :
      JoystickFeature(name),
      m_xIndex(xIndex),
      m_xInverted(xInverted),
      m_yIndex(yIndex),
      m_yInverted(yInverted),
      m_zIndex(zIndex),
      m_zInverted(zInverted)
    {
    }

    DriverAccelerometer(const JOYSTICK_FEATURE& feature) :
      JoystickFeature(feature),
      m_xIndex(feature.driver_accelerometer.x_index),
      m_xInverted(feature.driver_accelerometer.x_inverted),
      m_yIndex(feature.driver_accelerometer.y_index),
      m_yInverted(feature.driver_accelerometer.y_inverted),
      m_zIndex(feature.driver_accelerometer.z_index),
      m_zInverted(feature.driver_accelerometer.z_inverted)
    {
    }

    virtual ~DriverAccelerometer(void) { }

    virtual JoystickFeature* Clone(void) const { return new DriverAccelerometer(*this); }

    virtual bool Equals(const JoystickFeature* other) const
    {
      return JoystickFeature::Equals(other) &&
             m_xIndex    == static_cast<const DriverAccelerometer*>(other)->m_xIndex    &&
             m_xInverted == static_cast<const DriverAccelerometer*>(other)->m_xInverted &&
             m_yIndex    == static_cast<const DriverAccelerometer*>(other)->m_yIndex    &&
             m_yInverted == static_cast<const DriverAccelerometer*>(other)->m_yInverted &&
             m_zIndex    == static_cast<const DriverAccelerometer*>(other)->m_zIndex    &&
             m_zInverted == static_cast<const DriverAccelerometer*>(other)->m_zInverted;
    }

    virtual JOYSTICK_DRIVER_TYPE Type(void) const      { return JOYSTICK_DRIVER_TYPE_ACCELEROMETER; }
    int                          XIndex(void) const    { return m_xIndex; }
    bool                         XInverted(void) const { return m_xInverted; }
    int                          YIndex(void) const    { return m_yIndex; }
    bool                         YInverted(void) const { return m_yInverted; }
    int                          ZIndex(void) const    { return m_zIndex; }
    bool                         ZInverted(void) const { return m_zInverted; }

    void SetXIndex(int xIndex)        { m_xIndex    = xIndex; }
    void SetXInverted(bool xInverted) { m_xInverted = xInverted; }
    void SetYIndex(int yIndex)        { m_yIndex    = yIndex; }
    void SetYInverted(bool yInverted) { m_yInverted = yInverted; }
    void SetZIndex(int zIndex)        { m_zIndex    = zIndex; }
    void SetZInverted(bool zInverted) { m_zInverted = zInverted; }

    virtual void ToStruct(JOYSTICK_FEATURE& feature) const
    {
      JoystickFeature::ToStruct(feature);
      feature.driver_accelerometer.x_index    = m_xIndex;
      feature.driver_accelerometer.x_inverted = m_xInverted;
      feature.driver_accelerometer.y_index    = m_yIndex;
      feature.driver_accelerometer.y_inverted = m_yInverted;
      feature.driver_accelerometer.z_index    = m_zIndex;
      feature.driver_accelerometer.z_inverted = m_zInverted;
    }

  private:
    int  m_xIndex;
    bool m_xInverted;
    int  m_yIndex;
    bool m_yInverted;
    int  m_zIndex;
    bool m_zInverted;
  };

  /*!
   * ADDON::JoystickFeatureFactory
   *
   * Utility class to create joystick features polymorphically
   */
  class JoystickFeatureFactory
  {
  public:
    static JoystickFeature* Create(const JOYSTICK_FEATURE& feature)
    {
      switch (feature.driver_type)
      {
      case JOYSTICK_DRIVER_TYPE_BUTTON:        return new DriverButton(feature);
      case JOYSTICK_DRIVER_TYPE_HAT_DIRECTION: return new DriverHat(feature);
      case JOYSTICK_DRIVER_TYPE_SEMIAXIS:      return new DriverSemiAxis(feature);
      case JOYSTICK_DRIVER_TYPE_ANALOG_STICK:  return new DriverAnalogStick(feature);
      case JOYSTICK_DRIVER_TYPE_ACCELEROMETER: return new DriverAccelerometer(feature);
      default:
        break;
      }
      return NULL;
    }
  };
}
