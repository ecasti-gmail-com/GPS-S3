/*
   Icons:
   0 = Home
   1 = Food ( restaurant / cafe )
   2 = Point Of Interest
   3 = Transport ( Port/Airport/Train)
   4 = Hotel
   5 = Work
   6 = Shopping
   7 = Petrol Pump
   8 = Traffic signal
   9 = Settings
   10 = Car
   11 = Train
   12 = Toll
   13 = Crossorads
   14 = Sea
   15 = Peer 
   Images Generated on https://xbm.jazzychad.net/
*/

// Home, 10x10px
const unsigned char epd_bitmap_home[] PROGMEM = {
  0x78, 0xfc, 0xfc, 0xfc, 0xfe, 0xfd, 0xff, 0xff, 0xfe, 0xfd, 0x26, 0xfd,
  0x26, 0xfd, 0x26, 0xfd, 0xe6, 0xfd, 0xfe, 0xfd
};

// Food, 10x10px
const unsigned char epd_bitmap_food[] PROGMEM = {
  0x24, 0xfc, 0x48, 0xfc, 0x10, 0xfc, 0x7e, 0xff, 0xfe, 0xfe, 0xfe, 0xff,
  0x7c, 0xfc, 0x38, 0xfc, 0x83, 0xfd, 0xfe, 0xfc
};

// 'Point Of Interest, 10x10px
const unsigned char epd_bitmap_hotel[] PROGMEM = {
  0x89, 0x00, 0xc9, 0x01, 0xef, 0x03, 0x29, 0x02, 0xa9, 0x02, 0x20, 0x02,
  0xa0, 0x02, 0x20, 0x02, 0xa0, 0x02, 0xe0, 0x03
};
// 'Transport', 10x10px
const unsigned char epd_bitmap_transport[] PROGMEM = {
  0x40, 0xfc, 0x60, 0xfc, 0xf0, 0xfc, 0xf8, 0xfc, 0xfc, 0xfd, 0xfe, 0xfd,
  0xff, 0xff, 0x30, 0xfc, 0xff, 0xff, 0xfe, 0xfd
};
// 'Hotel, 10x10px
const unsigned char epd_bitmap_poi[] PROGMEM = {
  0x78, 0xfc, 0xfc, 0xfc, 0xce, 0xfd, 0x86, 0xfd, 0xce, 0xfd, 0xfc, 0xfc,
  0x78, 0xfc, 0x78, 0xfc, 0x30, 0xfc, 0x30, 0xfc
};

// 'Office', 10x10px
const unsigned char epd_bitmap_office[] PROGMEM = {
  0xc6, 0xfd, 0xc4, 0xff, 0x8e, 0xff, 0xd8, 0xfe, 0x70, 0xfc, 0x30, 0xfc,
  0x78, 0xfc, 0xcc, 0xfd, 0x86, 0xfc, 0x83, 0xfd
};

// 'shoopping', 10x10px
const unsigned char epd_bitmap_shopping[] PROGMEM = {
  0x01, 0xfc, 0xfe, 0xfd, 0x02, 0xfe, 0x02, 0xfe, 0x02, 0xfe, 0xfe, 0xfd,
  0x01, 0xfc, 0x01, 0xfc, 0xfe, 0xfd, 0x84, 0xfc
};

// 'Petrol Pump', 10x10px
const unsigned char epd_bitmap_petrol[] PROGMEM = {
  0x00, 0xfc, 0x38, 0xff, 0x44, 0xfd, 0x44, 0xfd, 0xd4, 0xfd, 0x54, 0xfd,
  0x54, 0xfd, 0x44, 0xfd, 0x44, 0xfc, 0xfe, 0xfc
};

// 'Traffic Signal', 10x10px
const unsigned char epd_bitmap_traffic_signal[] PROGMEM = {
  0xfc, 0xfc, 0x87, 0xff, 0xb6, 0xfd, 0xb4, 0xfc, 0x87, 0xff, 0xb6, 0xfd,
  0xb4, 0xfc, 0x84, 0xfc, 0xb6, 0xfd, 0xfc, 0xfc
};

// Settings 10x10px
const unsigned char epd_bitmap_settings[] PROGMEM = {
  0x30, 0xfc, 0xb6, 0xfd, 0xfe, 0xfd, 0xcc, 0xfc, 0x87, 0xff, 0x87, 0xff,
  0xcc, 0xfc, 0xfe, 0xfd, 0xb6, 0xfd, 0x30, 0xfc
};
// Settings 10x10px
const unsigned char epd_bitmap_car[] PROGMEM = {
  0x00, 0xfc, 0xf0, 0xff, 0x08, 0xfe, 0xfc, 0xff, 0x02, 0xfe, 0x05, 0xff,
  0xfb, 0xfe, 0x8e, 0xff, 0x00, 0xfc, 0x00, 0xfc
};
// Settings 10x10px
const unsigned char epd_bitmap_school[] PROGMEM = {
  0x8e, 0xfc, 0xc1, 0xfd, 0xef, 0xff, 0x28, 0xfe, 0xa7, 0xfe, 0x20, 0xfe,
  0xbe, 0xfe, 0x22, 0xfe, 0xaa, 0xfe, 0xea, 0xff
};
// Settings 10x10px
const unsigned char epd_bitmap_train[] PROGMEM = {
  0x08, 0xfc, 0x10, 0xfc, 0x20, 0xfc, 0xff, 0xfc, 0x80, 0xfd, 0x2a, 0xfe,
  0x00, 0xfe, 0xff, 0xff, 0x54, 0xfd, 0xff, 0xff
};
// Settings 10x10px
const unsigned char epd_bitmap_toll[] PROGMEM = {
  0xd8, 0xfc, 0xd8, 0xfc, 0x00, 0xfc, 0xfe, 0xff, 0x02, 0xfe, 0xd8, 0xfc,
  0xd8, 0xfc, 0xdc, 0xfd, 0xdc, 0xfd, 0xdc, 0xfd
};
// Settings 10x10px
const unsigned char epd_bitmap_crossroads[] PROGMEM = {
  0x30, 0xfc, 0xfc, 0xfd, 0x04, 0xfe, 0xfc, 0xfd, 0x30, 0xfc, 0xfe, 0xfc,
  0x81, 0xfc, 0xfe, 0xfc, 0x30, 0xfc, 0x30, 0xfc
};
// Settings 10x10px
const unsigned char epd_bitmap_sea[] PROGMEM = {
  0x00, 0xfc, 0x1c, 0xfc, 0x23, 0xfe, 0xc0, 0xfd, 0x1c, 0xfc, 0x23, 0xfe,
  0xc0, 0xfd, 0x1c, 0xfc, 0x23, 0xfe, 0xc0, 0xfd
};

const unsigned char epd_bitmap_man[] PROGMEM = {
  0x70, 0xfc, 0x70, 0xfc, 0x20, 0xfc, 0xfc, 0xfd, 0x20, 0xfc, 0x20, 0xfc,
  0x20, 0xfc, 0x50, 0xfc, 0x88, 0xfc, 0x04, 0xfd
};


const unsigned char* symbols_bitmap[] = {
  epd_bitmap_home,
  epd_bitmap_food,
  epd_bitmap_poi,
  epd_bitmap_transport,
  epd_bitmap_hotel,
  epd_bitmap_office,
  epd_bitmap_shopping,
  epd_bitmap_petrol,
  epd_bitmap_traffic_signal,
  epd_bitmap_settings,
  epd_bitmap_car,
  epd_bitmap_train,
  epd_bitmap_toll,
  epd_bitmap_crossroads,
  epd_bitmap_sea,
  epd_bitmap_man

};
