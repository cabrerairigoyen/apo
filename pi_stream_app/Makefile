apps += PiStream::App
app_headers += apps/pi_stream_app/pi_stream_app.h

app_pi_stream_src = $(addprefix apps/pi_stream_app/,\
  pi_stream_app.cpp \
  pi_stream_controller.cpp \
)

apps_src += $(app_pi_stream_src)

app_images += apps/pi_stream_app/pi_stream_icon.png

i18n_files += $(call i18n_without_universal_for,pi_stream_app/base)

$(eval $(call depends_on_image,apps/pi_stream_app/pi_stream_app.cpp,apps/pi_stream_app/pi_stream_icon.png))
