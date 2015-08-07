InfectionType is_{{drug | lower }}_susceptible(dBGraph* db_graph,
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
            {% if drug.name =="Erythromycin" %} boolean* any_erm_present,{% endif %}
             CalledVariant* called_variants,CalledGene* called_genes,
             CmdLine* cmd_line
				    )

{
  InfectionType I_permanent = Unsure;
  reset_antibiotic_info(abi);
  {% if drug.name =="Erythromycin" %} *any_erm_present=false;{% endif %}


  //setup antibiotic info object
  abi->ab = {{drug}};
  strbuf_append_str(abi->m_fasta, install_dir->buff);
  strbuf_append_str(abi->m_fasta, "data/{{selfer.species}}/antibiotics/{{drug|lower}}.fa");
  {% for gene_enum in drug.genes_resistance_induced_by %}
    abi->which_genes[{{loop.index0}}]={{gene_enum}};
  {% endfor %}
  abi->num_genes={{drug.num_genes}};
  abi->num_mutations = {{drug.num_mutations}};

  double epsilon = pow(1-err_rate, db_graph->kmer_size);
  load_antibiotic_mut_and_gene_info(db_graph,
				    file_reader,
				    abi,
				    rutils,
				    tmp_vob,
				    tmp_gi,
				    ignore_first, ignore_last, expected_covg,
				    install_dir);
  double max_sus_conf=0;
  double min_conf=9999999;  
  int i;
  Model best_model;
  boolean genotyped_present = false;
{% if drug.num_mutations > 1 %}
{% include 'src/predictor/common/mutations_iter.c' %}
{% elif drug.num_mutations == 1 %}
{% include 'src/predictor/common/single_mut.c' %}
{% endif %}
{% if drug.num_genes ==1 %}
{% include 'src/predictor/common/single_gene.c' %}
{% elif drug.num_genes  > 1 %}
{% include 'src/predictor/common/genes_iter.c' %}
{% endif %}  
{% if drug.has_epistatic_muts %}
{% include 'src/predictor/'+selfer.species+'/epistatic.c' %}
{% endif %}

  
  
  {% if drug.num_mutations > 0 %}
  if( (I_permanent==Resistant) || (I_permanent==MixedInfection) ) {
    return I_permanent;
  }
  else{
    if (any_allele_non_null==false)
      {
        return Unsure;
      }
    else if (max_sus_conf>MIN_CONFIDENCE_S)
      {
        return Susceptible;
      }
    else
      {
        return Unsure;
      }   
  }
  {% else %} 
  return I_permanent;
  {% endif %}

}
