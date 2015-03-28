/* routines to calibrate ellipsoid based sensor data */
#define ACCEL 0
#define MAG   1

/***** set MAXPOINTS, call addpoint, and read from bias and scale */

/* read and write data to eeprom (we really need the memory) */

/* offset in eeprom to store data */
#define ACCEL_OFFSET (void*)0x00
#define MAG_OFFSET (void*)0xA0  /* enough for 11 points plus scale and bias */

#if MAXPOINTS > 1000
#error "must adjust offsets in eeprom to fit this many points"
#endif

#define BIAS_OFFSET   0x00  /* bias first */
#define SCALE_OFFSET  0x0C  /* scale next */
#define POINTS_OFFSET 0x18  /* points next */

#define MAKE_ACCESSORS(NAME, OFFSET, SIZE) \
   static inline void read##NAME(void *val, uint8_t sensor) \
   { \
      if(sensor == ACCEL) \
         eeprom_read_block_safe(val, ACCEL_OFFSET + OFFSET, SIZE); \
      else \
         eeprom_read_block_safe(val, MAG_OFFSET + OFFSET, SIZE); \
   } \
   static inline void write##NAME(void *val, uint8_t sensor) \
   { \
      if(sensor == ACCEL) \
         eeprom_write_block_safe(val, ACCEL_OFFSET + OFFSET, SIZE); \
      else \
         eeprom_write_block_safe(val, MAG_OFFSET + OFFSET, SIZE); \
   }

/* a way to write just one point */
static inline void writepoint(void *val, uint8_t point, uint8_t sensor)
{
   const int len = sizeof(float) * 3, off = len * point;
   if(sensor == ACCEL)
      eeprom_write_block_safe(val, ACCEL_OFFSET + POINTS_OFFSET + off, len);
   else
      eeprom_write_block_safe(val, MAG_OFFSET + POINTS_OFFSET + off, len);
}

/* yuck on size, please clean */
MAKE_ACCESSORS(bias, BIAS_OFFSET, 0x0C)
MAKE_ACCESSORS(scale, SCALE_OFFSET, 0x0C)
MAKE_ACCESSORS(points, POINTS_OFFSET, (MAXPOINTS*3*4))
