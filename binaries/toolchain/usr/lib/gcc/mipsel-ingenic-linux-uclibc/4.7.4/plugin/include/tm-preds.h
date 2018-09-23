/* Generated automatically by the program 'build/genpreds'
   from the machine description file '../../gcc/config/mips/mips.md'.  */

#ifndef GCC_TM_PREDS_H
#define GCC_TM_PREDS_H

#ifdef HAVE_MACHINE_MODES
extern int general_operand (rtx, enum machine_mode);
extern int address_operand (rtx, enum machine_mode);
extern int register_operand (rtx, enum machine_mode);
extern int pmode_register_operand (rtx, enum machine_mode);
extern int scratch_operand (rtx, enum machine_mode);
extern int immediate_operand (rtx, enum machine_mode);
extern int const_int_operand (rtx, enum machine_mode);
extern int const_double_operand (rtx, enum machine_mode);
extern int nonimmediate_operand (rtx, enum machine_mode);
extern int nonmemory_operand (rtx, enum machine_mode);
extern int push_operand (rtx, enum machine_mode);
extern int pop_operand (rtx, enum machine_mode);
extern int memory_operand (rtx, enum machine_mode);
extern int indirect_operand (rtx, enum machine_mode);
extern int ordered_comparison_operator (rtx, enum machine_mode);
extern int comparison_operator (rtx, enum machine_mode);
extern int const_uns_arith_operand (rtx, enum machine_mode);
extern int uns_arith_operand (rtx, enum machine_mode);
extern int const_arith_operand (rtx, enum machine_mode);
extern int arith_operand (rtx, enum machine_mode);
extern int const_uimm6_operand (rtx, enum machine_mode);
extern int const_imm10_operand (rtx, enum machine_mode);
extern int reg_imm10_operand (rtx, enum machine_mode);
extern int sle_operand (rtx, enum machine_mode);
extern int sleu_operand (rtx, enum machine_mode);
extern int const_0_operand (rtx, enum machine_mode);
extern int reg_or_0_operand (rtx, enum machine_mode);
extern int const_1_operand (rtx, enum machine_mode);
extern int reg_or_1_operand (rtx, enum machine_mode);
extern int const_0_or_1_operand (rtx, enum machine_mode);
extern int const_2_or_3_operand (rtx, enum machine_mode);
extern int const_0_to_3_operand (rtx, enum machine_mode);
extern int qi_mask_operand (rtx, enum machine_mode);
extern int hi_mask_operand (rtx, enum machine_mode);
extern int si_mask_operand (rtx, enum machine_mode);
extern int and_load_operand (rtx, enum machine_mode);
extern int low_bitmask_operand (rtx, enum machine_mode);
extern int and_reg_operand (rtx, enum machine_mode);
extern int and_operand (rtx, enum machine_mode);
extern int d_operand (rtx, enum machine_mode);
extern int lo_operand (rtx, enum machine_mode);
extern int hilo_operand (rtx, enum machine_mode);
extern int fcc_reload_operand (rtx, enum machine_mode);
extern int muldiv_target_operand (rtx, enum machine_mode);
extern int pc_or_label_operand (rtx, enum machine_mode);
extern int const_call_insn_operand (rtx, enum machine_mode);
extern int call_insn_operand (rtx, enum machine_mode);
extern int splittable_const_int_operand (rtx, enum machine_mode);
extern int move_operand (rtx, enum machine_mode);
extern int cprestore_save_slot_operand (rtx, enum machine_mode);
extern int cprestore_load_slot_operand (rtx, enum machine_mode);
extern int consttable_operand (rtx, enum machine_mode);
extern int symbolic_operand (rtx, enum machine_mode);
extern int absolute_symbolic_operand (rtx, enum machine_mode);
extern int symbolic_operand_with_high (rtx, enum machine_mode);
extern int force_to_mem_operand (rtx, enum machine_mode);
extern int got_disp_operand (rtx, enum machine_mode);
extern int got_page_ofst_operand (rtx, enum machine_mode);
extern int tls_reloc_operand (rtx, enum machine_mode);
extern int symbol_ref_operand (rtx, enum machine_mode);
extern int stack_operand (rtx, enum machine_mode);
extern int macc_msac_operand (rtx, enum machine_mode);
extern int equality_operator (rtx, enum machine_mode);
extern int extend_operator (rtx, enum machine_mode);
extern int trap_comparison_operator (rtx, enum machine_mode);
extern int order_operator (rtx, enum machine_mode);
extern int mips_cstore_operator (rtx, enum machine_mode);
extern int small_data_pattern (rtx, enum machine_mode);
#endif /* HAVE_MACHINE_MODES */

#define CONSTRAINT_NUM_DEFINED_P 1
enum constraint_num
{
  CONSTRAINT__UNKNOWN = 0,
  CONSTRAINT_d,
  CONSTRAINT_t,
  CONSTRAINT_f,
  CONSTRAINT_h,
  CONSTRAINT_l,
  CONSTRAINT_x,
  CONSTRAINT_b,
  CONSTRAINT_c,
  CONSTRAINT_e,
  CONSTRAINT_j,
  CONSTRAINT_v,
  CONSTRAINT_y,
  CONSTRAINT_z,
  CONSTRAINT_A,
  CONSTRAINT_a,
  CONSTRAINT_B,
  CONSTRAINT_C,
  CONSTRAINT_D,
  CONSTRAINT_ka,
  CONSTRAINT_kf,
  CONSTRAINT_ks,
  CONSTRAINT_I,
  CONSTRAINT_J,
  CONSTRAINT_K,
  CONSTRAINT_L,
  CONSTRAINT_M,
  CONSTRAINT_N,
  CONSTRAINT_O,
  CONSTRAINT_P,
  CONSTRAINT_G,
  CONSTRAINT_Q,
  CONSTRAINT_R,
  CONSTRAINT_S,
  CONSTRAINT_T,
  CONSTRAINT_U,
  CONSTRAINT_W,
  CONSTRAINT_YG,
  CONSTRAINT_YA,
  CONSTRAINT_YB,
  CONSTRAINT_Yb,
  CONSTRAINT_Yh,
  CONSTRAINT_Yw,
  CONSTRAINT_Yx,
  CONSTRAINT__LIMIT
};

extern enum constraint_num lookup_constraint (const char *);
extern bool constraint_satisfied_p (rtx, enum constraint_num);

static inline size_t
insn_constraint_len (char fc, const char *str ATTRIBUTE_UNUSED)
{
  switch (fc)
    {
    case 'Y': return 2;
    case 'k': return 2;
    default: break;
    }
  return 1;
}

#define CONSTRAINT_LEN(c_,s_) insn_constraint_len (c_,s_)

extern enum reg_class regclass_for_constraint (enum constraint_num);
#define REG_CLASS_FROM_CONSTRAINT(c_,s_) \
    regclass_for_constraint (lookup_constraint (s_))
#define REG_CLASS_FOR_CONSTRAINT(x_) \
    regclass_for_constraint (x_)

extern bool insn_const_int_ok_for_constraint (HOST_WIDE_INT, enum constraint_num);
#define CONST_OK_FOR_CONSTRAINT_P(v_,c_,s_) \
    insn_const_int_ok_for_constraint (v_, lookup_constraint (s_))

#define CONST_DOUBLE_OK_FOR_CONSTRAINT_P(v_,c_,s_) \
    constraint_satisfied_p (v_, lookup_constraint (s_))

#define EXTRA_CONSTRAINT_STR(v_,c_,s_) \
    constraint_satisfied_p (v_, lookup_constraint (s_))

extern bool insn_extra_memory_constraint (enum constraint_num);
#define EXTRA_MEMORY_CONSTRAINT(c_,s_) insn_extra_memory_constraint (lookup_constraint (s_))

#define EXTRA_ADDRESS_CONSTRAINT(c_,s_) false

#endif /* tm-preds.h */
