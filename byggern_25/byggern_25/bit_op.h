/*
 * bit_op.h
 *
 * Created: 03.09.2014 11:29:43
 *  Author: boerikse
 */ 


#ifndef BIT_OP_H_
#define BIT_OP_H_

#define set_bit(reg,bit) (reg |= 1 << bit)
#define clear_bit(reg,bit) (reg &= ~(1 << bit))
#define toggle_bit(reg,bit) (reg ^= (1 << bit))
#define get_bit(reg,bit) ((reg & (1 << bit)) >> bit)
#define write_bit(c,reg,bit) (c ? set_bit(reg,bit) : clear_bit(reg,bit))
#define loop_until_bit_is_set( reg, bit ) while( !get_bit( reg, bit ) )
#define loop_until_bit_is_clear( reg, bit ) while( get_bit( reg, bit ) )


#endif /* BIT_OP_H_ */