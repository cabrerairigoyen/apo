#include "pi_stream_app.h"
#include "apps/apps_container.h"
#include "pi_stream_icon.h"
#include "apps/i18n.h"
#include <assert.h>

namespace PiStream {

I18n::Message App::Descriptor::name() {
  return I18n::Message::PiStreamApp;
}

I18n::Message App::Descriptor::upperName() {
  return I18n::Message::PiStreamAppCapital;
}

const Image * App::Descriptor::icon() {
  return ImageStore::PiStreamIcon;
}

App::Snapshot::Snapshot()
{
}

App * App::Snapshot::unpack(Container * container) {
  return new (container->currentAppBuffer()) App(this);
}

App::Descriptor * App::Snapshot::descriptor() {
  static Descriptor descriptor;
  return &descriptor;
}

void App::Snapshot::reset() {
}

App::App(Snapshot * snapshot) :
  ::App(snapshot, &m_piStreamController),
  m_piStreamController(this)
{
}

}
