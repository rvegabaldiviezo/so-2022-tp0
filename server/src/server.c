#include "server.h"

int main(void) {
	// 1) Crear el logger donde guardar logs del estado de cada funcionalidad
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	// 2) Iniciar servidor donde escuchar al cliente
	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");

	// 3) Esperar/Bloquear el proceso hasta que se conecte un Cliente.
	int cliente_fd = esperar_cliente(server_fd);

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
