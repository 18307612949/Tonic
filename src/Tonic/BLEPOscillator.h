//
//  BLEPOscillator.h
//  Tonic 
//
//  Created by Nick D on 7/21/13.
//
// See LICENSE.txt for license and usage information.
//

/*****************************************************
 *
 *  Abstract base Generator_ class for bandlimited
 *  waveform generation using minBLEP technique.
 *
 *  Sources here:
 *
 *  http://www.experimentalscene.com/articles/minbleps.php
 *  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=112
 *
 *****************************************************/


#ifndef __Tonic__BLEPOscillator__
#define __Tonic__BLEPOscillator__

// BOTH of thes must be powers of two
#define TONIC_MINBLEP_ZEROCROSSINGS   16
#define TONIC_MINBLEP_OVERSAMPLING    32

#include "Generator.h"
#include "DSPUtils.h"

namespace Tonic {
  
  namespace Tonic_ {

    class BLEPOscillator_ : public Generator_{
      
    protected:
      
      // Input generators
      Generator freqGen_;
      TonicFrames freqFrames_;
      
      // TODO: Hardsync?
      
      // Lookup table
      static TonicFloat * minBLEP_;
      static int minBLEPlength_;
      
      // phase accumulator and fast phasor
      double phase_;
      
      // ring buffer and accumulator
      TonicFloat * ringBuf_;
      TonicFloat accum_;
      int lBuffer_; // ring buffer length
      int iBuffer_; // current index
      int nInit_; // number of initialzed samples in ring buffer
            
      // add a BLEP to the ring buffer at the specified offset
      inline void addBLEP(TonicFloat offset, TonicFloat scale)
      {
        int i;
        float bufOffset = TONIC_MINBLEP_OVERSAMPLING*offset;
        
        TonicFloat f;
        TonicFloat * outptr = ringBuf_ + iBuffer_;
        TonicFloat * inptr = minBLEP_ + (int)bufOffset;
        TonicFloat * bufEnd = ringBuf_ + lBuffer_;
        
        float frac = fmodf(bufOffset,1.0);
        
        
        // add
        for (i=0; i<nInit_; i++, inptr += TONIC_MINBLEP_OVERSAMPLING, outptr++)
        {
          if (outptr >= bufEnd) outptr = ringBuf_;
          f = lerp(inptr[0], inptr[1], frac);
          *outptr += scale * (1.f - f);
        }
        
        // copy
        for (; i<lBuffer_-1; i++, inptr += TONIC_MINBLEP_OVERSAMPLING, outptr++)
        {
          if (outptr >= bufEnd) outptr = ringBuf_;
          f = lerp(inptr[0], inptr[1], frac);
          *outptr = scale * (1.f - f);
        }
        
        nInit_ = lBuffer_-1;
      }
  
    public:

      BLEPOscillator_();
      ~BLEPOscillator_();
      
      void setFreqGen(Generator gen) { freqGen_ = gen; };
      
    };
    
  }
}

#endif /* defined(__Tonic__BLEPOscillator__) */


