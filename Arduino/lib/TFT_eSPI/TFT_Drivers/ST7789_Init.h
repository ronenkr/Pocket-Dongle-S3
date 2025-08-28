
// This is the command sequence that initialises the ST7789 driver
//
// This setup information uses simple 8 bit SPI writecommand() and writedata() functions
//
// See ST7735_Setup.h file for an alternative format

#ifndef INIT_SEQUENCE_3
{
  writecommand(0x11);
delay(120);

writecommand(0x36);
writedata(0x00);
writedata(TFT_MAD_COLOR_ORDER);

writecommand(0x3A);
writedata(0x05);


writecommand(0xB2);
writedata(0x1F);
writedata(0x1F);
writedata(0x00);
writedata(0x33);
writedata(0x33);

writecommand(0xB7);
writedata(0x00); // VGH=12.2V,VGL=-7.16V

writecommand(0xBB);
writedata(0x3F);

writecommand(0xC0);
writedata(0x2C);

writecommand(0xC2);
writedata(0x01);

writecommand(0xC3);
writedata(0x0f); //4.3V

writecommand(0xC4);
writedata(0x20);  //VDV, 0x20:0v

writecommand(0xC6);
writedata(0x13);

writecommand(0xD0);
writedata(0xA4);
writedata(0xA1);


writecommand(0xE0);
writedata(0xF0);
writedata(0x06);
writedata(0x0D);
writedata(0x0B);
writedata(0x0A);
writedata(0x07);
writedata(0x2E);
writedata(0x43);
writedata(0x45);
writedata(0x38);
writedata(0x14);
writedata(0x13);
writedata(0x25);
writedata(0x29);

writecommand(0xE1);
writedata(0xF0);
writedata(0x07);
writedata(0x0A);
writedata(0x08);
writedata(0x07);
writedata(0x23);
writedata(0x2E);
writedata(0x33);
writedata(0x44);
writedata(0x3A);
writedata(0x16);
writedata(0x17);
writedata(0x26);
writedata(0x2C);

writecommand(0xE4);
writedata(0x1d);
writedata(0x00);
writedata(0x00);

writecommand(0x21);
delay(120);

writecommand(0x29);


writecommand(0x2c);

#ifdef TFT_BL
  // Turn on the back-light LED
  digitalWrite(TFT_BL, HIGH);
  pinMode(TFT_BL, OUTPUT);
#endif
}


#else
// TTGO ESP32 S3 T-Display
{
  writecommand(ST7789_SLPOUT);   // Sleep out
  delay(120);

  writecommand(ST7789_NORON);    // Normal display mode on

  //------------------------------display and color format setting--------------------------------//
  writecommand(ST7789_MADCTL);
  writedata(TFT_MAD_COLOR_ORDER);

 // writecommand(ST7789_RAMCTRL);
 // writedata(0x00);
 // writedata(0xE0); // 5 to 6 bit conversion: r0 = r5, b0 = b5

  writecommand(ST7789_COLMOD);
  writedata(0x55);
  delay(10);

  //--------------------------------ST7789V Frame rate setting----------------------------------//
  writecommand(ST7789_PORCTRL);
  writedata(0x0b);
  writedata(0x0b);
  writedata(0x00);
  writedata(0x33);
  writedata(0x33);

  writecommand(ST7789_GCTRL);      // Voltages: VGH / VGL
  writedata(0x75);

  //---------------------------------ST7789V Power setting--------------------------------------//
  writecommand(ST7789_VCOMS);
  writedata(0x28);		// JLX240 display datasheet

  writecommand(ST7789_LCMCTRL);
  writedata(0x2C);

  writecommand(ST7789_VDVVRHEN);
  writedata(0x01);

  writecommand(ST7789_VRHS);       // voltage VRHS
  writedata(0x1F);

  writecommand(ST7789_FRCTR2);
  writedata(0x13);

  writecommand(ST7789_PWCTRL1);
  writedata(0xa7);

  writecommand(ST7789_PWCTRL1);
  writedata(0xa4);
  writedata(0xa1);

  writecommand(0xD6);
  writedata(0xa1);

  //--------------------------------ST7789V gamma setting---------------------------------------//
  writecommand(ST7789_PVGAMCTRL);
  writedata(0xf0);
  writedata(0x05);
  writedata(0x0a);
  writedata(0x06);
  writedata(0x06);
  writedata(0x03);
  writedata(0x2b);
  writedata(0x32);
  writedata(0x43);
  writedata(0x36);
  writedata(0x11);
  writedata(0x10);
  writedata(0x2b);
  writedata(0x32);

  writecommand(ST7789_NVGAMCTRL);
  writedata(0xf0);
  writedata(0x08);
  writedata(0x0c);
  writedata(0x0b);
  writedata(0x09);
  writedata(0x24);
  writedata(0x2b);
  writedata(0x22);
  writedata(0x43);
  writedata(0x38);
  writedata(0x15);
  writedata(0x16);
  writedata(0x2f);
  writedata(0x37);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  end_tft_write();
  delay(120);
  begin_tft_write();

  writecommand(ST7789_DISPON);    //Display on
  delay(120);

#ifdef TFT_BL
  // Turn on the back-light LED
  digitalWrite(TFT_BL, HIGH);
  pinMode(TFT_BL, OUTPUT);
#endif
}
#endif
