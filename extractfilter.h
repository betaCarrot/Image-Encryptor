#ifndef EXTRACTFILTER_H
#define EXTRACTFILTER_H

#include "animationfilter.h"
#include <QLinkedList>

/*!
 * \brief The filter for extracting the secret image from an GIF animated image.
 */
class ExtractFilter: public AnimationFilter {
public:
    virtual void apply() override;
};

#endif // EXTRACTFILTER_H
