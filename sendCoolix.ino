const uint16_t kIrLed = 16;
IRac ac(kIrLed);

void IrSend(boolean power, uint8_t mode, boolean celsius, short degrees, uint8_t fanspeed, const char* swingv, boolean light, boolean beep, boolean econo, boolean filter, boolean turbo, boolean quiet, boolean clean, int protocol, String manufacturer) {


//  Serial.println("------------COOLIX-----------");
//  Serial.print("power: ");
//  Serial.println(power);
//  Serial.print("mode: ");
//  Serial.println(mode);
//  Serial.print("celsius: ");
//  Serial.println(celsius);
//  Serial.print("degrees: ");
//  Serial.println(degrees);
//  Serial.print("fanspeed: ");
//  Serial.println(fanspeed);
//  Serial.print("swingv: ");
//  Serial.println(swingv);
//  Serial.print("light: ");
//  Serial.println(light);
//  Serial.print("beep: ");
//  Serial.println(beep);
//  Serial.print("econo: ");
//  Serial.println(econo);
//  Serial.print("filter: ");
//  Serial.println(filter);
//  Serial.print("turbo: ");
//  Serial.println(turbo);
//  Serial.print("quiet: ");
//  Serial.println(quiet);
//  Serial.print("clean: ");
//  Serial.println(clean);
//  Serial.println("-----------COOLIX------------");

  ac.next.protocol = (decode_type_t)protocol;
  ac.next.power = power;
  ac.next.model = -1; //JUST USE IT. because it works
  ac.next.mode = IRCoolixAC::toCommonMode(mode);
  ac.next.celsius = celsius;  // Use Celsius for temp units. False = Fahrenheit
  ac.next.degrees = degrees;
  ac.next.fanspeed = IRCoolixAC::toCommonFanSpeed(fanspeed);
  ac.next.swingv = stdAc::swingv_t::kOff;
  ac.next.swingh = stdAc::swingh_t::kOff;
  ac.next.light = light;  // Turn off any LED/Lights/Display that we can.
  ac.next.beep = beep;  // Turn off any beep from the A/C if we can.
  ac.next.econo = econo;  // Turn off any economy modes if we can.
  ac.next.filter = filter;  // Turn off any Ion/Mold/Health filters if we can.
  ac.next.turbo = turbo;  // Don't use any turbo/powerful/etc modes.
  ac.next.quiet = quiet;  // Don't use any quiet/silent/etc modes.
  ac.next.sleep = -1;  // Don't set any sleep time or modes.
  ac.next.clean = clean;  // Turn off any Cleaning options if we can.
  ac.next.clock = -1;  // Don't set any current time if we can avoid it.
  telemetry("Sending remote codes");
  ac.sendAc();
}
