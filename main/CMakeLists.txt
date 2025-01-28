set(srcs "main.c" "udp_listener.c" "udp_client.c" "uart.c")

idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")
