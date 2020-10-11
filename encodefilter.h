#ifndef ENCODEFILTER_H
#define ENCODEFILTER_H

#include "basefilter.h"
#include "matrix.h"

/*!
 * \brief The filter for encoding a secret image into a base image.
 */
class EncodeFilter: public BaseFilter {
public:
    void setSecret(QImage);
    virtual void apply() override;
private:
    QImage secret; /*!< The secret image */
};

#endif // ENCODEFILTER_H
