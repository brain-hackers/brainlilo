#include <map>
#include <string>

const std::map<std::wstring, std::wstring> models = {
    {L"ED-CD2",  L"gen1.bin"},   // AC880
    {L"ED-CD2D", L"gen1.bin"},   // AC830
    {L"ED-CD1",  L"gen1.bin"},   // TC980
    {L"ED-CD4",  L"gen1.bin"},   // GC590
    {L"ED-CD5",  L"gen1.bin"},   // AC890
    {L"ED-CD6",  L"gen1.bin"},   // AC900
    {L"ED-CD9",  L"gen1.bin"},   // GC610
    {L"ED-CD8",  L"gen1.bin"},   // AC910
    {L"ED-CD10", L"gen1.bin"},   // AC920
    {L"ED-NJ1",  L"gen1.bin"},   // G4000
    {L"ED-NS1",  L"gen1.bin"},   // G5000
    {L"ED-NS1",  L"gen1.bin"},   // HC1
    {L"ED-NB1",  L"gen1.bin"},   // A9000
    {L"ED-NA1",  L"gen1.bin"},   // A7000
    {L"ED-NS1P", L"gen1.bin"},   // G5100
    {L"ED-NJ2",  L"gen2.bin"},   // G4200
    {L"ED-NH2",  L"gen2.bin"},   // G5200
    {L"ED-NH2",  L"gen2.bin"},   // HC2
    {L"ED-NB2",  L"gen2.bin"},   // A9200
    {L"ED-NA2",  L"gen2.bin"},   // A7200
    {L"ED-NA3",  L"gen2.bin"},   // A7300
    {L"SD-GH1",  L"gen2.bin"},   // GX500
    {L"ED-NH3",  L"gen2.bin"},   // G5300
    {L"ED-NH3",  L"gen2.bin"},   // HC3
    {L"ED-NB3",  L"gen2.bin"},   // A9300
    {L"ED-NR3",  L"gen2.bin"},   // A9100
    {L"SD-GJ1",  L"gen2.bin"},   // GX300
    {L"ED-NQ3",  L"gen2.bin"},   // A7400
    {L"ED-SJ1",  L"gen3_1.bin"}, // SJ1
    {L"ED-SH1",  L"gen3_1.bin"}, // SH1
    {L"ED-HC4",  L"gen3_1.bin"}, // HC4
    {L"ED-SB1",  L"gen3_1.bin"}, // SB1
    {L"ED-SA1",  L"gen3_1.bin"}, // SA1
    {L"ED-SR1",  L"gen3_1.bin"}, // SR1
    {L"ED-SJ2",  L"gen3_2.bin"}, // SJ2
    {L"ED-SH2",  L"gen3_2.bin"}, // SH2
    {L"ED-HC5",  L"gen3_2.bin"}, // HC5
    {L"ED-SB2",  L"gen3_2.bin"}, // SB2
    {L"ED-SA2",  L"gen3_2.bin"}, // SA2
    {L"ED-SJ3",  L"gen3_3.bin"}, // SJ3
    {L"ED-SH3",  L"gen3_3.bin"}, // SH3
    {L"ED-HC6",  L"gen3_3.bin"}, // HC6
    {L"ED-SB3",  L"gen3_3.bin"}, // SB3
    {L"ED-SA3",  L"gen3_3.bin"}, // SA3
    {L"ED-SJ4",  L"gen3_4.bin"}, // SJ4
    {L"ED-SH4",  L"gen3_4.bin"}, // SH4
    {L"ED-H77",  L"gen3_4.bin"}, // H7700
    {L"ED-SB4",  L"gen3_4.bin"}, // SB4
    {L"ED-SA4",  L"gen3_4.bin"}, // SA4
    {L"ED-SR2",  L"gen3_4.bin"}, // SR2
    {L"ED-SJ5",  L"gen3_5.bin"}, // SJ5
    {L"ED-SH5",  L"gen3_5.bin"}, // SH5
    {L"ED-H78",  L"gen3_5.bin"}, // H7800
    {L"ED-SB5",  L"gen3_5.bin"}, // SB5
    {L"ED-SA5",  L"gen3_5.bin"}, // SA5
    {L"ED-AJ1",  L"gen3_5.bin"}, // AJ1
    {L"ED-AA1",  L"gen3_5.bin"}, // AA1
    {L"ED-SH6",  L"gen3_6.bin"}, // SH6
    {L"ED-SS6",  L"gen3_6.bin"}, // SS6
    {L"ED-H80",  L"gen3_6.bin"}, // H8000
    {L"ED-SB6",  L"gen3_6.bin"}, // SB6
    {L"ED-AJ2",  L"gen3_6.bin"}, // AJ2
    {L"ED-AA2",  L"gen3_6.bin"}, // AA2
    {L"ED-SH7",  L"gen3_7.bin"}, // SH7
    {L"ED-SS7",  L"gen3_7.bin"}, // SS7
    {L"ED-H91",  L"gen3_7.bin"}, // H9100
    {L"ED-H81",  L"gen3_7.bin"}, // H8100
    {L"ED-SB7",  L"gen3_7.bin"}, // SB7
    {L"ED-SR3",  L"gen3_7.bin"}, // SR3
};

typedef enum
{
    UnknownGen,
    Gen1,
    Gen2,
    Gen3_1,
    Gen3_2,
    Gen3_3,
    Gen3_4,
    Gen3_5,
    Gen3_6,
    Gen3_7,
} BrainGen;