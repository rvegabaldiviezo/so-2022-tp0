#include "client.h"
#include "utils.h"
#include <readline/readline.h>
#include <string.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente, Escribi: "Hola! Soy un log"

	log_info(logger,"Hola! Soy un log");//log_destroy(logger);

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	// Loggeamos el valor de config
		char* key = "IP_CPU";
		int response = config_has_property(config,key);
		if(response >0){
			ip = config_get_string_value(config,key);
			log_info(logger,ip);

		}
		key = "CLAVE";
		response = config_has_property(config,key);
		if(response >0){
			valor = config_get_string_value(config,key);
			log_info(logger,valor);

		}
		key = "PUERTO_CPU";
		response = config_has_property(config,key);
		if(response >0){
			puerto = config_get_string_value(config,key);
			log_info(logger,puerto);

		}

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	/* ADVERTENCIA: Antes de continuar, tenemos que asegurarnos
	 * que el servidor esté corriendo para poder conectarnos a él
	 * => HAY QUE LEVANTARLO, en la carpeta de server, compilar
	 * y ejecutar el server.c */

	// Creamos una conexión hacia el servidor, retorna el socket_cliente
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	// Agregamos log_create
	nuevo_logger = log_create("./tp0.log","LogsTP", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
    nuevo_config = config_create("./cliente.config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// Lee la linea que escribamos por consola. Retorna el string leido.
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while( strcmp(leido,"") != 0){
		log_info(logger, leido);
		leido = readline("> ");
	}
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
		// 1 - Creo un Paquete
		paquete = crear_paquete();
		// 2- Leer de Consola
		leido = readline("> ");
		while( strcmp(leido,"") != 0){
		// 3 - Agregar al paqute
			int size = string_length(leido) + 1;
			agregar_a_paquete(paquete, leido, size);
		// 4 - Leer la sig linea.
			leido = readline("> ");
		}
	// Enviar el paquete dada una conexion
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	log_destroy(logger);

	config_destroy(config);

	liberar_conexion(conexion);
}
