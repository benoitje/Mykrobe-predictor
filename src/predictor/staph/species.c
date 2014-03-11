/*
 * Copyright 2014 Zamin Iqbal (zam@well.ox.ac.uk)
 * 
 *
 * **********************************************************************
 *
 * This file is part of myKrobe.
 *
 * myKrobe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * myKrobe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with myKrobe.  If not, see <http://www.gnu.org/licenses/>.
 *
 * **********************************************************************
 */
/*
  species.c
*/


// system headers
#include <stdlib.h>
#include <limits.h>

// third party headers
#include <string_buffer.h>

#include "build.h" 
#include "element.h"
#include "seq.h"
#include "open_hash/hash_table.h"
#include "dB_graph.h"
#include "species.h"
#include "gene_presence.h"
#include "genotyping_known.h"


void map_species_enum_to_str(Staph_species sp, StrBuf* sbuf)
{
  if (sp==Scapitis)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.capitis");
    }
  else if (sp==Scaprae)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.caprae");
    }
  else if (sp==Sepidermidis)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.epidermidis");
    }
  else if (sp==Sequorum)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.equorum");
    }
  else if (sp==Shaemolyticus)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.haemolyticus");
    }
  else if (sp==Shominis)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.hominis");
    }
  else if (sp==Slugdunensis)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.lugdunensis");
    }
  else if (sp==Smassiliensis)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.massiliensis");
    }
  else if (sp==Spettenkofer)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.pettenkofer");
    }
  else if (sp==Spseudintermedius)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.pseudintermedius");
    }
  else if (sp==Ssaprophyticus)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.saprophyticus");
    }
  else if (sp==Ssimiae)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.simiae");
    }
  else if (sp==Ssimulans)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"Ssimulans");
    }
  else if (sp==Ssphgb0015)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.sphgb0015");
    }
  else if (sp==Sspoj82)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.spoj82");
    }
  else if (sp==Aureus)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.aureus");
    }
  else if (sp==Swarneri)
    {
      strbuf_reset(sbuf);
      strbuf_append_str(sbuf,"S.warneri");
    }
  else
    {
      die("Coding error - I would expect the compiler to prevent assigning a bad enum value\n");
    }
  
}


Staph_species get_species(dBGraph *db_graph,int max_branch_len )
{
  // Define the paths to the possible species
  StrBuf* species_file_paths[17];
  species_file_paths[0] = strbuf_create("../data/species/Scapitis_unique_branches.fasta");
  species_file_paths[1] = strbuf_create("../data/species/Scaprae_unique_branches.fasta");
  species_file_paths[2] = strbuf_create("../data/species/Sepidermidis_unique_branches.fasta");
  species_file_paths[3] = strbuf_create("../data/species/Sequorum_unique_branches.fasta");
  species_file_paths[4] = strbuf_create("../data/species/Shaemolyticus_unique_branches.fasta");
  species_file_paths[5] = strbuf_create("../data/species/Shominis_unique_branches.fasta");
  species_file_paths[6] = strbuf_create("../data/species/Slugdunensis_unique_branches.fasta");
  species_file_paths[7] = strbuf_create("../data/species/Smassiliensis_unique_branches.fasta");
  species_file_paths[8] = strbuf_create("../data/species/Spettenkofer_unique_branches.fasta");
  species_file_paths[9] = strbuf_create("../data/species/Spseudintermedius_unique_branches.fasta");
  species_file_paths[10] = strbuf_create("../data/species/Ssaprophyticus_unique_branches.fasta");
  species_file_paths[11] = strbuf_create("../data/species/Ssimiae_unique_branches.fasta");
  species_file_paths[12] = strbuf_create("../data/species/Ssimulans_unique_branches.fasta");
  species_file_paths[13] = strbuf_create("../data/species/S_sp_hgb0015_unique_branches.fasta");
  species_file_paths[14] = strbuf_create("../data/species/S_sp_oj82_unique_branches.fasta");
  species_file_paths[15] = strbuf_create("../data/species/staph_unique_branches.fasta");
  species_file_paths[16] = strbuf_create("../data/species/S_warneri_unique_branches.fasta");

  int i;
  double pcov[17]; // for storing the percentage coverage of each reference
  int sumpcov;
  int number_of_reads;


  AlleleInfo* ai = alloc_allele_info();
  FILE* fp;

  //----------------------------------
  // allocate the memory used to read the sequences
  //----------------------------------
  Sequence * seq = malloc(sizeof(Sequence));
  if (seq == NULL){
    die("Out of memory trying to allocate Sequence");
  }
  alloc_sequence(seq,max_branch_len,LINE_MAX);

  //We are going to load all the bases into a single sliding window 
  KmerSlidingWindow* kmer_window = malloc(sizeof(KmerSlidingWindow));
  if (kmer_window==NULL)
    {
      die("Failed to malloc kmer sliding window");
    }


  CovgArray* working_ca = alloc_and_init_covg_array(max_branch_len);
  dBNode** array_nodes = (dBNode**) malloc(sizeof(dBNode*)*max_branch_len);
  Orientation* array_or =(Orientation*)  malloc(sizeof(Orientation)*max_branch_len);
  kmer_window->kmer = (BinaryKmer*) malloc(sizeof(BinaryKmer)*(max_branch_len-db_graph->kmer_size+1));
  if (kmer_window->kmer==NULL)
    {
      die("Failed to malloc kmer_window->kmer");
    }
  kmer_window->nkmers=0;
  //create file readers
  int file_reader_fasta(FILE * fp, Sequence * seq, int max_read_length, boolean new_entry, boolean * full_entry){
    long long ret;
    int offset = 0;
    if (new_entry == false){
      offset = db_graph->kmer_size;
    }
    ret =  read_sequence_from_fasta(fp,seq,max_read_length,new_entry,full_entry,offset);
    
    return ret;
  }
  
  
  
  if ( (array_nodes==NULL) || (array_or==NULL))
    {
      die("Cannot alloc array of nodes or of orientations");
    }
  
  for (i = 0; i < 17; i++)
    {
      
      
      fp = fopen(species_file_paths[i]->buff, "r");
      if (fp==NULL)
	{
	  die("Cannot open this file");
	}
      
      
      // while the entry is valid iterate through the fasta file
      number_of_reads = 0;
      int num_kmers=0;
      sumpcov = 0;
      do {
	
	num_kmers= get_next_single_allele_info(fp, db_graph, ai,
					       seq, kmer_window,
					       &file_reader_fasta,
					       array_nodes, array_or, 
					       working_ca, max_branch_len);
	number_of_reads = number_of_reads + 1;
	sumpcov = sumpcov + ai->percent_nonzero;
	
      } while ( num_kmers>0);
      if (number_of_reads>0)
	{
	  pcov[i] = sumpcov / number_of_reads;
	}
      else
	{
	  pcov[i]=0;
	}
      
    }

  
  free_allele_info(ai);
  free(array_nodes);
  free(array_or);
  free_covg_array(working_ca);
  free(kmer_window->kmer);
  free(kmer_window);
  free_sequence(&seq);

  for (i=0; i<17; i++)
    {
      strbuf_free(species_file_paths[i]);
    }
  // Look at the max of the pcov
  int c,location;
  double maximum=0;
  for (c = 0; c < 17; c++)
    {
      if (pcov[c] > maximum)
      {
         maximum  = pcov[c];
         location = c;
      }
    }
  //  printf("Maximum element is present at location %i and it's value is %f.\n", location, maximum);

  Staph_species species_out =  location;
  return species_out;

}