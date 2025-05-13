#include<sstream>
#include<tests/commandBufferToStr.hpp>
#include<tests/testCommon.hpp>

namespace tests{

std::string commandTypeToStr(CommandType const&type){
  switch(type){
    case CommandType::CLEAR           :return "CLEAR"           ;
    case CommandType::DRAW            :return "DRAW"            ;
    case CommandType::SET_DRAW_ID     :return "SET_DRAW_ID"     ;
    case CommandType::BIND_FRAMEBUFFER:return "BIND_FRAMEBUFFER";
    case CommandType::BIND_PROGRAM    :return "BIND_PROGRAM"    ;
    case CommandType::BIND_VERTEXARRAY:return "BIND_VERTEXARRAY";
    case CommandType::SUB_COMMAND     :return "SUB_COMMAND"     ;
    case CommandType::EMPTY           :return "EMPTY"           ;
  }
  return "";
}

std::string clearCommandToStr(size_t p,uint32_t i,ClearCommand const&cmd){
  std::stringstream ss;
  ss << padding(p)<<"cb.commands["<<i<<"].data.clearCommand.color      = glm::vec4"<<str(cmd.color) <<";"<<std::endl;
  ss << padding(p)<<"cb.commands["<<i<<"].data.clearCommand.depth      = "<<cmd.depth               <<";"<<std::endl;
  ss << padding(p)<<"cb.commands["<<i<<"].data.clearCommand.clearColor = "<<str(cmd.clearColor)     <<";"<<std::endl;
  ss << padding(p)<<"cb.commands["<<i<<"].data.clearCommand.clearDepth = "<<str(cmd.clearDepth)     <<";"<<std::endl;
  return ss.str();
}

std::string drawCommandToStr(size_t p,uint32_t i,DrawCommand const&cmd){
  std::stringstream ss;
  ss << padding(p) << "cb.commands["<<i<<"].data.drawCommand.backfaceCulling = "<<str(cmd.backfaceCulling) <<";" << std::endl;
  ss << padding(p) << "cb.commands["<<i<<"].data.drawCommand.nofVertices     = "<<cmd.nofVertices          <<";" << std::endl;
  return ss.str();
}

std::string setDrawIdToStr(size_t p,uint32_t i,SetDrawIdCommand const&cmd){
  std::stringstream ss;
  ss << padding(p)<<"cb.commands["<<i<<"].data.setDrawIdCommand.id = "<<str(cmd.id) <<";"<<std::endl;
  return ss.str();
}

std::string bindFramebufferToStr(size_t p,uint32_t i,BindFramebufferCommand const&cmd){
  std::stringstream ss;
  ss << padding(p)<<"cb.commands["<<i<<"].data.bindFramebufferCommand.id = "<<str(cmd.id) <<";"<<std::endl;
  return ss.str();
}

std::string bindProgramToStr(size_t p,uint32_t i,BindProgramCommand const&cmd){
  std::stringstream ss;
  ss << padding(p)<<"cb.commands["<<i<<"].data.bindProgramCommand.id = "<<str(cmd.id) <<";"<<std::endl;
  return ss.str();
}

std::string bindVertexArrayToStr(size_t p,uint32_t i,BindVertexArrayCommand const&cmd){
  std::stringstream ss;
  ss << padding(p)<<"cb.commands["<<i<<"].data.bindVertexArrayCommand.id = "<<str(cmd.id) <<";"<<std::endl;
  return ss.str();
}

std::string subCommandToStr(size_t p,uint32_t i,SubCommand const&cmd){
  std::stringstream ss;
  ss << padding(p)<<"cb.commands["<<i<<"].data.subCommand.commandBuffer = {" << std::endl;
  ss << commandBufferToStr(p+2,*cmd.commandBuffer);
  ss << padding(p)<<"}" << std::endl;
  return ss.str();
}

std::string commandToStr(size_t p,uint32_t i,Command const&cmd){
  std::stringstream ss;
  ss << padding(p) << "cb.commands["<<i<<"].type = CommandType::" << commandTypeToStr(cmd.type) << ";" << std::endl;
  switch(cmd.type){
    case CommandType::CLEAR:
      ss << clearCommandToStr(p,i,cmd.data.clearCommand);
      break;
    case CommandType::DRAW:
      ss << drawCommandToStr(p,i,cmd.data.drawCommand);
      break;
    case CommandType::SET_DRAW_ID:
      ss << setDrawIdToStr(p,i,cmd.data.setDrawIdCommand);
      break;
    case CommandType::BIND_FRAMEBUFFER:
      ss << bindFramebufferToStr(p,i,cmd.data.bindFramebufferCommand);
      break;
    case CommandType::BIND_PROGRAM:
      ss << bindProgramToStr(p,i,cmd.data.bindProgramCommand);
      break;
    case CommandType::BIND_VERTEXARRAY:
      ss << bindVertexArrayToStr(p,i,cmd.data.bindVertexArrayCommand);
      break;
    case CommandType::SUB_COMMAND:
      ss << subCommandToStr(p,i,cmd.data.subCommand);
      break;
    case CommandType::EMPTY:
      break;
  }
  return ss.str();
}

std::string commandBufferToStr(size_t p,CommandBuffer const&cb){
  std::stringstream ss;
  ss << padding(p) << "CommandBuffer cb;" << std::endl;
  ss << padding(p) << "cb.nofCommands = " << cb.nofCommands << ";" << std::endl;
  for(uint32_t i=0;i<cb.nofCommands;++i)
    ss<<commandToStr(p,i,cb.commands[i]);
  return ss.str();
}

}
