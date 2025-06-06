/*	BSD Socket UDP Listener

	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"
#include "esp_netif.h" // IP2STR
#include "lwip/sockets.h"

extern MessageBufferHandle_t xMessageBufferTx;
extern size_t xItemSize;

static const char *TAG = "LISTENER";

void udp_listner(void *pvParameters) {
	ESP_LOGI(TAG, "Start CONFIG_UDP_LISTEN_PORT=%d", CONFIG_UDP_LISTEN_PORT);

	// set up address to recvfrom
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(CONFIG_UDP_LISTEN_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); /* receive message from ANY */
	//addr.sin_addr.s_addr = inet_addr("0.0.0.0"); /* receive message from ANY */

	// create the socket
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.
	LWIP_ASSERT("sock >= 0", sock >= 0);

#if 0
	// set option
	int broadcast=1;
	int ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);
	LWIP_ASSERT("ret >= 0", ret >= 0);
#endif

	// bind socket
	int ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	LWIP_ASSERT("ret >= 0", ret >= 0);

	// senderInfo data
	struct sockaddr_in senderInfo;
	socklen_t senderInfoLen = sizeof(senderInfo);
	while(1) {
		char buffer[xItemSize];
		memset(buffer, 0, sizeof(buffer));
		int received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderInfo, &senderInfoLen);
		ESP_LOGI(TAG, "recvfrom received=%d errno=%d", received, errno);
		if (received < 0) {
			ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
			break;
		}
		if (received > 0) {
			ESP_LOGI(TAG, "recvfrom buffer=[%.*s]", received, buffer);
			char senderstr[16];
			inet_ntop(AF_INET, &senderInfo.sin_addr, senderstr, sizeof(senderstr));
			ESP_LOGI(TAG, "recvfrom : %s, port=%d", senderstr, ntohs(senderInfo.sin_port));
			if (buffer[received-1] != 0x0a) {
				buffer[received] = 0x0a;
				received++;
			}
			size_t sended = xMessageBufferSend(xMessageBufferTx, buffer, received, 100);
			if (sended != received) {
				ESP_LOGE(pcTaskGetName(NULL), "xMessageBufferSend fail received=%d sended=%d", received, sended);
				break;
			}
		}
	} // end while

	// close socket
	ret = close(sock);
	vTaskDelete( NULL );

}

