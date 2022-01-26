#include "core/base_widget.h"

using namespace IonaDesktop::Core;

void BaseWidget::setupMask()
{
    QPixmap pixmap_mask;
    pixmap_mask.load(":/mask/mask.png");
    this->setMask(pixmap_mask.mask());
}
