#ifndef PI_STREAM_APP_H
#define PI_STREAM_APP_H

#include <escher.h>
#include "pi_stream_controller.h"

namespace PiStream {

class App : public ::App {
public:
  class Descriptor : public ::App::Descriptor {
  public:
    I18n::Message name() override;
    I18n::Message upperName() override;
    const Image * icon() override;
  };
  class Snapshot : public ::App::Snapshot {
  public:
    Snapshot();
    App * unpack(Container * container) override;
    void reset() override;
    Descriptor * descriptor() override;
  };
private:
  App(Snapshot * snapshot);
  PiStreamController m_piStreamController;
};

}

#endif
