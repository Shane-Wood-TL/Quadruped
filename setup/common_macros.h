/**
 * @brief macro to keep the code from progressing if a condition is not met
 */
#define assertS(x) while((x)){}

/**
 * @brief map function to map a value from one range to another
 * 
 * @param a : numerical value to change the range of
 * @param b : the lower bound of the current range
 * @param c : the upper bound of the current range
 * @param w : the lower bound of the new range
 * @param q : the upper bound of the new range
 * 
 * @note No checking is done on the input or output side, if a is outside of [b,c] the results
 * will not be as expected, given that the input is also not an expected value
 */
#define map(a,b,c,w,q) ((w)+(((a) - (b)) * ((q) - (w)))/((c)-(b)))

/**
 * @brief macro to get the 0th bit of a byte
 */
#define get_bit_0(x) ((x) & 0x01)
/**
 * @brief macro to get the 1st bit of a byte
 */
#define get_bit_1(x) (((x) >> 1) & 0x01)
/**
 * @brief macro to get the 2st bit of a byte
 */
#define get_bit_2(x) (((x) >> 2) & 0x01)
/**
 * @brief macro to get the 3rd bit of a byte
 */
#define get_bit_3(x) (((x) >> 3) & 0x01)
/**
 * @brief macro to get the 4th bit of a byte
 */
#define get_bit_4(x) (((x) >> 4) & 0x01)
/**
 * @brief macro to get the 5th bit of a byte
 */
#define get_bit_5(x) (((x) >> 5) & 0x01)
/**
 * @brief macro to get the 6th bit of a byte
 */
#define get_bit_6(x) (((x) >> 6) & 0x01)
/**
 * @brief macro to get the 7th bit of a byte
 */
#define get_bit_7(x) (((x) >> 7) & 0x01)
