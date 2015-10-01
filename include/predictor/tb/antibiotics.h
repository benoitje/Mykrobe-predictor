/*
 * Copyright 2014 Zamin Iqbal (zam@well.ox.ac.uk)
 *
 * antibiotics.h
*/

#ifndef ANTIBIOTICS_H_
#define ANTIBIOTICS_H_

#include "stdlib.h"
#include "string_buffer.h"
#include "gene_presence.h"
#include "genotyping_known.h"
#include "known_mutations.h"
#include "file_reader.h"
#include "mut_models.h"
#include "json.h"
#include "global.h"
#include "cmd_line.h"


//#define MAX_MUTS_IN_ANY_GENE 129
#define MAX_LEN_MUT_ALLELE 100



#define MIN_PERC_COVG_BLAZ 30
#define MIN_PERC_COVG_DFRK 50
#define MIN_PERC_COVG_FUSBC 50
#define MIN_PERC_COVG_STANDARD 50
#define MIN_PERC_COVG_VIRULENCE 30 
#define GENE_THRESH_pvl 70
#define MAX_GENES_PER_ANTIBIOTIC 10

typedef enum 
 {
   NoDrug=0,
   
   amikacin=1,
   
   capreomycin=2,
   
   ethambutol=3,
   
   isoniazid=4,
   
   kanamycin=5,
   
   pyrazinamide=6,
   
   quinolones=7,
   
   rifampicin=8,
   
   streptomycin=9
   
  } Antibiotic;




char* map_gene_to_drug_resistance(GenePresenceGene gene);
void map_antibiotic_enum_to_str(Antibiotic ab, StrBuf* name);



typedef struct
{
  Antibiotic ab;
  StrBuf* m_fasta; //mutations
  StrBuf** g_fasta;//array of StrBuf*s, one per gene (which may contain many exemplars of that gene)
  int num_mutations;
  int num_genes;
  Var** vars;// array of length NUM_KNOWN_MUTATIONS];//indexed by enum of mutation names
  GeneInfo** genes;// array of length NUM_GENE_PRESENCE_GENES];//indexed by enum of gene names
  int* which_genes;
} AntibioticInfo;






AntibioticInfo* alloc_antibiotic_info();
void free_antibiotic_info(AntibioticInfo* abi);
void reset_antibiotic_info(AntibioticInfo* abi);

void  load_antibiotic_mutation_info_on_sample(FILE* fp,
					      dBGraph* db_graph,
					      int (*file_reader)(FILE * fp, 
								 Sequence * seq, 
								 int max_read_length, 
								 boolean new_entry, 
								 boolean * full_entry),
					      AntibioticInfo* abi,
					      ReadingUtils* rutils,
					      VarOnBackground* tmp_vob,
					      int ignore_first, int ignore_last, int expected_covg);

void load_antibiotic_gene_presence_info_on_sample(FILE* fp,
						  dBGraph* db_graph,
						  int (*file_reader)(FILE * fp, 
								     Sequence * seq, 
								     int max_read_length, 
								     boolean new_entry, 
								     boolean * full_entry),
						  AntibioticInfo* abi,
						  ReadingUtils* rutils,
						  GeneInfo* tmp_gi);


void load_antibiotic_mut_and_gene_info(dBGraph* db_graph,
				       int (*file_reader)(FILE * fp, 
							  Sequence * seq, 
							  int max_read_length, 
							  boolean new_entry, 
							  boolean * full_entry),
				       AntibioticInfo* abi,
				       ReadingUtils* rutils,
				       VarOnBackground* tmp_vob,
				       GeneInfo* tmp_gi,
				       int ignore_first, 
				       int ignore_last, 
				       int expected_covg,
				       StrBuf* install_dir);


InfectionType is_amikacin_susceptible(dBGraph* db_graph,
				  int (*file_reader)(FILE * fp, 
						     Sequence * seq, 
						     int max_read_length, 
						     boolean new_entry, 
						     boolean * full_entry),
				  ReadingUtils* rutils,
				  VarOnBackground* tmp_vob,
				  GeneInfo* tmp_gi,
				  AntibioticInfo* abi,
				  StrBuf* install_dir,
				  int ignore_first, int ignore_last, int expected_covg,
				  double lambda_g, double lambda_e, double err_rate,
            	  				  
				   CalledVariant* called_variants,CalledGene* called_genes,
				   CmdLine* cmd_line
				  );

InfectionType is_capreomycin_susceptible(dBGraph* db_graph,
				  int (*file_reader)(FILE * fp, 
						     Sequence * seq, 
						     int max_read_length, 
						     boolean new_entry, 
						     boolean * full_entry),
				  ReadingUtils* rutils,
				  VarOnBackground* tmp_vob,
				  GeneInfo* tmp_gi,
				  AntibioticInfo* abi,
				  StrBuf* install_dir,
				  int ignore_first, int ignore_last, int expected_covg,
				  double lambda_g, double lambda_e, double err_rate,
            	  				  
				   CalledVariant* called_variants,CalledGene* called_genes,
				   CmdLine* cmd_line
				  );

InfectionType is_ethambutol_susceptible(dBGraph* db_graph,
				  int (*file_reader)(FILE * fp, 
						     Sequence * seq, 
						     int max_read_length, 
						     boolean new_entry, 
						     boolean * full_entry),
				  ReadingUtils* rutils,
				  VarOnBackground* tmp_vob,
				  GeneInfo* tmp_gi,
				  AntibioticInfo* abi,
				  StrBuf* install_dir,
				  int ignore_first, int ignore_last, int expected_covg,
				  double lambda_g, double lambda_e, double err_rate,
            	  				  
				   CalledVariant* called_variants,CalledGene* called_genes,
				   CmdLine* cmd_line
				  );

InfectionType is_isoniazid_susceptible(dBGraph* db_graph,
				  int (*file_reader)(FILE * fp, 
						     Sequence * seq, 
						     int max_read_length, 
						     boolean new_entry, 
						     boolean * full_entry),
				  ReadingUtils* rutils,
				  VarOnBackground* tmp_vob,
				  GeneInfo* tmp_gi,
				  AntibioticInfo* abi,
				  StrBuf* install_dir,
				  int ignore_first, int ignore_last, int expected_covg,
				  double lambda_g, double lambda_e, double err_rate,
            	  				  
				   CalledVariant* called_variants,CalledGene* called_genes,
				   CmdLine* cmd_line
				  );

InfectionType is_kanamycin_susceptible(dBGraph* db_graph,
				  int (*file_reader)(FILE * fp, 
						     Sequence * seq, 
						     int max_read_length, 
						     boolean new_entry, 
						     boolean * full_entry),
				  ReadingUtils* rutils,
				  VarOnBackground* tmp_vob,
				  GeneInfo* tmp_gi,
				  AntibioticInfo* abi,
				  StrBuf* install_dir,
				  int ignore_first, int ignore_last, int expected_covg,
				  double lambda_g, double lambda_e, double err_rate,
            	  				  
				   CalledVariant* called_variants,CalledGene* called_genes,
				   CmdLine* cmd_line
				  );

InfectionType is_pyrazinamide_susceptible(dBGraph* db_graph,
				  int (*file_reader)(FILE * fp, 
						     Sequence * seq, 
						     int max_read_length, 
						     boolean new_entry, 
						     boolean * full_entry),
				  ReadingUtils* rutils,
				  VarOnBackground* tmp_vob,
				  GeneInfo* tmp_gi,
				  AntibioticInfo* abi,
				  StrBuf* install_dir,
				  int ignore_first, int ignore_last, int expected_covg,
				  double lambda_g, double lambda_e, double err_rate,
            	  				  
				   CalledVariant* called_variants,CalledGene* called_genes,
				   CmdLine* cmd_line
				  );

InfectionType is_quinolones_susceptible(dBGraph* db_graph,
				  int (*file_reader)(FILE * fp, 
						     Sequence * seq, 
						     int max_read_length, 
						     boolean new_entry, 
						     boolean * full_entry),
				  ReadingUtils* rutils,
				  VarOnBackground* tmp_vob,
				  GeneInfo* tmp_gi,
				  AntibioticInfo* abi,
				  StrBuf* install_dir,
				  int ignore_first, int ignore_last, int expected_covg,
				  double lambda_g, double lambda_e, double err_rate,
            	  				  
				   CalledVariant* called_variants,CalledGene* called_genes,
				   CmdLine* cmd_line
				  );

InfectionType is_rifampicin_susceptible(dBGraph* db_graph,
				  int (*file_reader)(FILE * fp, 
						     Sequence * seq, 
						     int max_read_length, 
						     boolean new_entry, 
						     boolean * full_entry),
				  ReadingUtils* rutils,
				  VarOnBackground* tmp_vob,
				  GeneInfo* tmp_gi,
				  AntibioticInfo* abi,
				  StrBuf* install_dir,
				  int ignore_first, int ignore_last, int expected_covg,
				  double lambda_g, double lambda_e, double err_rate,
            	  				  
				   CalledVariant* called_variants,CalledGene* called_genes,
				   CmdLine* cmd_line
				  );

InfectionType is_streptomycin_susceptible(dBGraph* db_graph,
				  int (*file_reader)(FILE * fp, 
						     Sequence * seq, 
						     int max_read_length, 
						     boolean new_entry, 
						     boolean * full_entry),
				  ReadingUtils* rutils,
				  VarOnBackground* tmp_vob,
				  GeneInfo* tmp_gi,
				  AntibioticInfo* abi,
				  StrBuf* install_dir,
				  int ignore_first, int ignore_last, int expected_covg,
				  double lambda_g, double lambda_e, double err_rate,
            	  				  
				   CalledVariant* called_variants,CalledGene* called_genes,
				   CmdLine* cmd_line
				  );



void print_antibiotic_susceptibility(dBGraph* db_graph,
					int (*file_reader)(FILE * fp, 
							   Sequence * seq, 
							   int max_read_length, 
							   boolean new_entry, 
							   boolean * full_entry),
					ReadingUtils* rutils,
					VarOnBackground* tmp_vob,
					GeneInfo* tmp_gi,
					AntibioticInfo* abi,
					InfectionType (*func)(dBGraph* db_graph,
							int (*file_reader)(FILE * fp, 
									   Sequence * seq, 
									   int max_read_length, 
									   boolean new_entry, 
									   boolean * full_entry),
							ReadingUtils* rutils,
							VarOnBackground* tmp_vob,
							GeneInfo* tmp_gi,
							AntibioticInfo* abi,
							StrBuf* install_dir,
							int ignore_first, int ignore_last, int expected_covg,
							double lambda_g, double lambda_e, double err_rate,
							CalledVariant* called_variants,CalledGene* called_genes,
							CmdLine* cmd_line),
					StrBuf* tmpbuf,
					StrBuf* install_dir,
					int ignore_first, int ignore_last, int expected_covg,
					double lambda_g, double lambda_e, double err_rate, CmdLine* cmd_line, boolean output_last,
					CalledVariant* called_variants,CalledGene* called_genes
					);









#endif