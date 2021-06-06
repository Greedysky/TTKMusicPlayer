//
//  swap.h
//  org2dat
//
//  Created by Vincent Spader on 6/21/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

static __inline__ uint32_t Endian_Swap32(uint32_t x) {
        return((x<<24)|((x<<8)&0x00FF0000)|((x>>8)&0x0000FF00)|(x>>24));
}

static __inline__ uint16_t Endian_Swap16(uint16_t x) {
        return((x<<8)|(x>>8));
}

#define org_btoh_16(x) Endian_Swap16(x)
#define org_btoh_32(x) Endian_Swap32(x)

#define org_ltoh_16(x) x
#define org_ltoh_32(x) x
