#ifndef DECODEFILTER_H
#define DECODEFILTER_H

#include "basefilter.h"
#include "matrix.h"

/*!
 * \brief The filter for decoding the secret image from a cipher image.
 */
class DecodeFilter: public BaseFilter {
public:
    virtual void apply() override;
};

#endif // DECODEFILTER_H
