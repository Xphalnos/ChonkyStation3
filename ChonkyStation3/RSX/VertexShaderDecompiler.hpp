#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <string>
#include <sstream>
#include <BitField.hpp>
#include <unordered_map>
#include <format>
#include <RSXShaderOpcodes.hpp>


class VertexShaderDecompiler {
public:
    struct VertexInstruction {
        union {
            u32 raw;
            BitField<15, 6, u32> temp_dst_idx;
            BitField<30, 1, u32> is_output;
        } w0;
        union {
            u32 raw;
            BitField<0,  8,  u32> src0_hi;
            BitField<8,  4,  u32> input_src_idx;
            BitField<12, 10, u32> const_src_idx;
            BitField<22, 5,  u32> vector_opc;
            BitField<27, 5,  u32> scalar_opc;
        } w1;
        union {
            u32 raw;
            BitField<0, 6,  u32> src2_hi;
            BitField<6, 17, u32> src1;
            BitField<23, 9, u32> src0_lo;
        } w2;
        union {
            u32 raw;
            BitField<2,  5,  u32> dst;
            BitField<13, 1,  u32> w;
            BitField<14, 1,  u32> z;
            BitField<15, 1,  u32> y;
            BitField<16, 1,  u32> x;
            BitField<21, 11, u32> src2_lo;
        } w3;
    };

    union VertexSource {
        u32 raw;
        BitField<0, 2,  u32> type;
        BitField<2, 6,  u32> temp_src_idx; 
        BitField<8, 2,  u32> w;
        BitField<10, 2, u32> z;
        BitField<12, 2, u32> y;
        BitField<14, 2, u32> x;
        BitField<16, 1, u32> neg;
    };

    MAKE_LOG_FUNCTION(log, vertex_shader);

    std::string decompile(std::vector<u32> shader_data);
    
    void declareFunction(std::string name, std::string code, std::string& shader);
    void markInputAsUsed(std::string name, int location);
    void markOutputAsUsed(std::string name, int location);
    //void markConstAsUsed(std::string name, int location);

    std::string source(VertexSource& src, VertexInstruction* instr);
    std::string dest(VertexInstruction* instr);
    std::string mask(VertexInstruction* instr, int& num_lanes);
    std::string getType(const int num_lanes);

    bool used_inputs[16];
    bool used_outputs[16];
    std::string inputs;
    std::string outputs;

    enum VERTEX_SOURCE_TYPE {
        TEMP = 1,
        INPUT = 2,
        CONST = 3
    };

    const std::string input_names[16] = {
        "in_pos",
        "in_weight",
        "in_normal",
        "in_diff_color",
        "in_spec_color",
        "in_fog",
        "in_point_size",
        "in_unk7",
        "in_tex0",
        "in_tex1",
        "in_tex2",
        "in_tex3",
        "in_tex4",
        "in_tex5",
        "in_tex6",
        "in_tex7"
    };

    const std::string output_names[16] = {
        "out_pos",
        "out_col0",
        "out_col1",
        "out_bfc0",
        "out_bfc1",
        "out_fog",
        "out_point_size",
        "out_tex0",
        "out_tex1",
        "out_tex2",
        "out_tex3",
        "out_tex4",
        "out_tex5",
        "out_tex6",
        "out_tex7",
        "out_unk15"
    };

    std::unordered_map<u8, std::string> vertex_vector_opcodes {
        { RSXVertex::VECTOR::NOP, "NOP" },
        { RSXVertex::VECTOR::MOV, "MOV" },
        { RSXVertex::VECTOR::MUL, "MUL" },
        { RSXVertex::VECTOR::ADD, "ADD" },
        { RSXVertex::VECTOR::MAD, "MAD" },
        { RSXVertex::VECTOR::DP3, "DP3" },
        { RSXVertex::VECTOR::DPH, "DPH" },
        { RSXVertex::VECTOR::DP4, "DP4" },
        { RSXVertex::VECTOR::DST, "DST" },
        { RSXVertex::VECTOR::MIN, "MIN" },
        { RSXVertex::VECTOR::MAX, "MAX" },
        { RSXVertex::VECTOR::SLT, "SLT" },
        { RSXVertex::VECTOR::SGE, "SGE" },
        { RSXVertex::VECTOR::ARL, "ARL" },
        { RSXVertex::VECTOR::FRC, "FRC" },
        { RSXVertex::VECTOR::FLR, "FLR" },
        { RSXVertex::VECTOR::SEQ, "SEQ" },
        { RSXVertex::VECTOR::SFL, "SFL" },
        { RSXVertex::VECTOR::SGT, "SGT" },
        { RSXVertex::VECTOR::SLE, "SLE" },
        { RSXVertex::VECTOR::SNE, "SNE" },
        { RSXVertex::VECTOR::STR, "STR" },
        { RSXVertex::VECTOR::SSG, "SSG" },
        { RSXVertex::VECTOR::TXL, "TXL" },
    };

    std::unordered_map<u8, std::string> vertex_scalar_opcodes {
        { RSXVertex::SCALAR::NOP, "NOP" },
        { RSXVertex::SCALAR::MOV, "MOV" },
        { RSXVertex::SCALAR::RCP, "RCP" },
        { RSXVertex::SCALAR::RCC, "RCC" },
        { RSXVertex::SCALAR::RSQ, "RSQ" },
        { RSXVertex::SCALAR::EXP, "EXP" },
        { RSXVertex::SCALAR::LOG, "LOG" },
        { RSXVertex::SCALAR::LIT, "LIT" },
        { RSXVertex::SCALAR::BRA, "BRA" },
        { RSXVertex::SCALAR::BRI, "BRI" },
        { RSXVertex::SCALAR::CAL, "CAL" },
        { RSXVertex::SCALAR::CLI, "CLI" },
        { RSXVertex::SCALAR::RET, "RET" },
        { RSXVertex::SCALAR::LG2, "LG2" },
        { RSXVertex::SCALAR::EX2, "EX2" },
        { RSXVertex::SCALAR::SIN, "SIN" },
        { RSXVertex::SCALAR::COS, "COS" },
        { RSXVertex::SCALAR::BRB, "BRB" },
        { RSXVertex::SCALAR::CLB, "CLB" },
        { RSXVertex::SCALAR::PSH, "PSH" },
        { RSXVertex::SCALAR::POP, "POP" },
    };
};