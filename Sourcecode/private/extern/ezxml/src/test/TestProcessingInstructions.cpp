#include <sstream>

#include "Catch.h"
#include "ezxml/ezxml.h"
#include "Files.h"

namespace ezxmltest
{
    struct Instruction
    {
        std::string name;
        std::string value;
        int elementNodeID{};
    };

    using Instructions = std::vector<Instruction>;


    inline void
    recurse( const ezxml::XElement& current, int currentNodeID, Instructions& ioInstructions )
    {
        const auto name = current.getName();
        if( current.getIsProcessingInstruction() )
        {
            Instruction i;
            i.name = current.getName();
            i.value = current.getValue();
            i.elementNodeID = currentNodeID;
            ioInstructions.emplace_back( std::move( i ) );
        }
        const auto xtype = current.getType();
        if( xtype == ezxml::XElementType::element )
        {
            auto iter = current.beginWithProcessingInstructions();
            int childNodeID = currentNodeID + 1;
            const auto end = current.end();
            for( ; iter != end; ++iter, ++childNodeID )
            {
                recurse( *iter, childNodeID, ioInstructions );
            }
        }
    }

}

TEST_CASE( "SimpleProcessingInstruction" )
{
    using namespace ezxml;
    using namespace ezxmltest;
    const auto fp = filepath( SIMPLE );
    const auto xdoc = XFactory::makeXDoc();
    xdoc->loadFile( fp );
    const auto root = xdoc->getRoot();
    Instructions instructions;
    ezxmltest::recurse( *root, 0, instructions );
    REQUIRE( 1 == instructions.size() );
    const auto& instruction = instructions.at( 0 );
    CHECK( "InstructionName" == instruction.name );
    CHECK( "InstructionValue" == instruction.value );
    CHECK( 2 == instruction.elementNodeID );
}


TEST_CASE( "FreezingProcessingInstructions" )
{
    using namespace ezxml;
    using namespace ezxmltest;
    const auto fp = filepath( FREEZING_SMALL );
    const auto xdoc = XFactory::makeXDoc();
    xdoc->loadFile( fp );
    const auto root = xdoc->getRoot();
    Instructions instructions;
    ezxmltest::recurse( *root, 0, instructions );
    REQUIRE( 4 == instructions.size() );
    size_t index = 0;
    auto instruction = instructions.at( index );
    CHECK( "DoletFinale" == instruction.name );
    CHECK( "Unknown text expression 31 at part P4, measure X1, edu 3072" == instruction.value );
    CHECK( 23 == instruction.elementNodeID );

    index = 1;
    instruction = instructions.at( index );
    CHECK( "DoletFinale" == instruction.name );
    CHECK( "Unknown text expression 31 at part P4, measure X3, edu 3072" == instruction.value );
    CHECK( 27 == instruction.elementNodeID );

    index = 2;
    instruction = instructions.at( index );
    CHECK( "DoletFinale" == instruction.name );
    CHECK( "Unknown text expression 31 at part P4, measure X6, edu 512" == instruction.value );
    CHECK( 27 == instruction.elementNodeID );

    index = 3;
    instruction = instructions.at( index );
    CHECK( "DoletFinale" == instruction.name );
    CHECK( "Unknown text expression 31 at part P4, measure X7, edu 1536" == instruction.value );
    CHECK( 28 == instruction.elementNodeID );
}