#include "lab4.h"

FILE *abre_arquivo(const char *arquivo){
	FILE *ptr_arq = fopen(arquivo, "r");

	int t_fseek, t_ftell;
	t_fseek = fseek(ptr_arq, 0, SEEK_END);
	t_ftell = ftell(ptr_arq);
	if(t_fseek == t_ftell){
		return NULL;
	}
	return ptr_arq;
}

int fecha_arquivo(FILE *arq){
	if(arq == NULL){
		return 1;
	}

	if(feof(arq) != 0){
		return 1;
	}

	int i = fclose(arq);
	return i;
}

times_t *le_times(const char *arquivo, unsigned int *numero_de_times){
	if(numero_de_times == NULL){
		return NULL;
	}	

	FILE *ptr_arq = abre_arquivo(arquivo);
	if (ptr_arq == NULL){
		*numero_de_times = 0;
		fecha_arquivo(ptr_arq);
		return NULL;
	}
	
	int fim_arq = fseek(ptr_arq, 0, SEEK_END);
	//printf("fseet: %d \n", fim_arq);
	int tam_arq = ftell(ptr_arq);
	//printf("fteel: %d \n", tam_arq);
	*numero_de_times = tam_arq/sizeof(times_t);
	printf("numero_times: %d\n", *numero_de_times);
	
	times_t *times = (times_t*)malloc(sizeof(times_t)*(*numero_de_times));
	if(times == NULL){
		*numero_de_times = 0;
		fecha_arquivo(ptr_arq);
		return NULL;
	}
	
	int i = 0;
	rewind(ptr_arq);
	/*while(i <(*numero_de_times)){
		fread(&times[i],sizeof(times_t), 1, ptr_arq);
		printf("time: %s, estado:%s, Jogador posicao:%d \n",times[i].nome, times[i].estado, times[i].jogadores[0].posicao);
		i++;
	}*/
	
	fread(&times[i],sizeof(times_t), *numero_de_times, ptr_arq);
	
	int fecha_arq = fecha_arquivo(ptr_arq);
	if(fecha_arq != 0){ 

		*numero_de_times = 0;
		return NULL;
	}
	return times;
}
void encontra_jogador_cartao_vermelho_e_jogos(times_t *times, unsigned int num_times, jogador_t **jogador, times_t **time){

	if(time == NULL || jogador == NULL){
		return;
	}

	float relacao = 0, maior_relacao = 0;
	int i_time, j_jogador;

	for(int i = 0; i< num_times ; i++){
		for(int j = 0; j< NUM_JOGADORES; j++){

			relacao = times[i].jogadores[j].cartoes_vermelhos/times[i].jogadores[j].numero_jogos;
			printf("\ntime %d, jogador %s, relacao %f\n", i, times[i].jogadores[j].nome, relacao);

			if(relacao > maior_relacao){

				maior_relacao = relacao;
				i_time = i;
				j_jogador = j;

			}
		}
	}
	printf("time: %d e jogador: %d\n",i_time, j_jogador);
	*time =&times[i_time];
	*jogador =&times[i_time].jogadores[j_jogador];
	return;

}
void encontra_jogador_artilheiro(times_t *times, unsigned int num_times, jogador_t **jogador, times_t **time){
	if(time == NULL || jogador == NULL){
		return;
	}

	int num_gols = 0, maior_num_gols = 0;
	int i_time, j_jogador;

	for(int i = 0; i<num_times; i++){
		for(int j = 0; j< NUM_JOGADORES; j++){

			num_gols = times[i].jogadores[j].gols_marcados;
			printf("time %d, jogador %s, gols %d\n", i, times[i].jogadores[j].nome, num_gols);

			if(num_gols > maior_num_gols){

				maior_num_gols = num_gols;
				i_time = i;
				j_jogador = j;
			}
		}	
	}
	//*time = &times[i_time];
	//*jogador = &times[i_time].jogadores[j_jogador];
}

void encontra_jogador_mais_eficiente(times_t *times, unsigned int num_times, jogador_t **jogador, times_t **time){
	if(time == NULL || jogador == NULL){
		return;
	}

	float relacao = 0, maior_relacao = 0;
	int i_time, j_jogador;

	for(int i = 0; i<num_times; i++){
		for(int j = 0; j< NUM_JOGADORES; j++){

			relacao = times[i].jogadores[j].gols_marcados/times[i].jogadores[j].numero_jogos;
			printf("time %d, jogador %s, relacao %f\n", i, times[i].jogadores[j].nome, relacao);

			if(relacao > maior_relacao){

				maior_relacao = relacao;
				i_time = i;
				j_jogador = j;
			}
		}
	}
	//*time = &times[i_time];
	//*jogador = &times[i_time].jogadores[j_jogador];
}

void encontra_time_com_mais_titulos(times_t *times, unsigned int num_times, times_t **time){
	if(time == NULL){
		return;
	}

	int relacao_titulos = 0, maior_relacao = 0;
	int i_time;

	for(int i = 0; i<num_times; i++){

		relacao_titulos = (times[i].numero_tit_brasileiros + times[i].numero_copas_brasil + times[i].numero_tit_libertadores + times[i].numero_tit_estadual) / times[i].ano_fundacao;
		printf("time %s, relacao_titulos %d\n", times[i].nome, relacao_titulos);
		
		if(relacao_titulos > maior_relacao){

			maior_relacao = relacao_titulos;
			i_time = i;
		}
	}
	//*time = &times[i_time];
}

void encontra_goleiro_que_defendeu_mais_penaltis(times_t *times, unsigned int num_times, jogador_t **jogador, times_t **time){

	if(time == NULL){
		return;
	}

	int relacao_goleiro = 0, maior_relacao = 0;
	int i_time, j_jogador;

	for(int i = 0; i<num_times; i++){
		for(int j=0; j<NUM_JOGADORES; j++){

			printf("time %d, jogador %d\n", i, times[i].jogadores[j].posicao);
			if(times[i].jogadores[j].posicao == 0){
				relacao_goleiro = times[i].jogadores[j].penalti_defendidos/times[i].jogadores[j].numero_jogos;
			
				if(relacao_goleiro > maior_relacao){

					maior_relacao = relacao_goleiro;
					i_time = i;
					j_jogador = j;
				}
			}

		}
	}
	//*time = &times[i_time];
	//*jogador = &times[i_time].jogadores[j_jogador];
}
	


void encontra_jogador_mais_agressivo(times_t *times, unsigned int num_times, jogador_t **jogador, times_t **time){
	if(time == NULL || jogador == NULL){
		return;
	}

	float relacao = 0, maior_relacao = 0;
	int i_time, j_jogador;

	for(int i = 0; i<num_times; i++){
		for(int j = 0; j< NUM_JOGADORES; j++){

			relacao = (5*(times[i].jogadores[j].cartoes_vermelhos)) + (1*(times[i].jogadores[j].cartoes_amarelos)) + (0.2*(times[i].jogadores[j].faltas_cometidas));

			if(relacao > maior_relacao){

				maior_relacao = relacao;
				i_time = i;
				j_jogador = j;
			}
		}
	}
	//*time = &times[i_time];
	//*jogador = &times[i_time].jogadores[j_jogador];
}


int main(){
	FILE *ptr_arq = abre_arquivo("arq.dat");

	int i = fecha_arquivo(ptr_arq);

	unsigned int num_times;
	
	times_t *times = le_times("arq.dat", &num_times);
	//printf("%d num_times:\n", num_times);

	times_t *time_main;
	jogador_t *jogadores_main;

	encontra_time_com_mais_titulos(times, num_times,&time_main);

	//encontra_jogador_cartao_vermelho_e_jogos(times, num_times,&jogadores_main, &time_main);
	/*encontra_jogador_artilheiro(times, num_times,jogadores, time_main);
	encontra_jogador_mais_eficiente(times, num_times,jogadores, time_main);
	encontra_time_com_mais_titulos(times, num_times,time_main);
	encontra_goleiro_que_defendeu_mais_penaltis(times, num_times,jogadores, time_main);*/
	free(times);
	return 0;
}

