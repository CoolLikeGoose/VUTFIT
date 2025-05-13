-- -- uart_rx.vhd: UART controller - receiving (RX) side
-- -- Author(s): Nikita Smirnov (xsmirn02)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
port(
   CLK      :   in std_logic;
   RST      :   in std_logic;
   DIN      :   in std_logic;
   DOUT     :   out std_logic_vector(7 downto 0);
   DOUT_VLD :   out std_logic
);
end UART_RX;  

-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is
   signal  validation        :   std_logic;
   signal  clk_cnt           :   std_logic_vector(4 downto 0);
   signal  result_cnt        :   std_logic_vector(3 downto 0);
   signal  result_cnt_update :   std_logic;
   signal  clk_cnt_update    :   std_logic;

begin
   -- Instance of RX FSM
   fsm: entity work.UART_RX_FSM
   port map(
      CLK               =>  CLK,
      RST               =>  RST,
    
      DIN               =>  DIN,
      DOUT_VLD          =>  validation,

      CLK_CNT           =>  clk_cnt,
      RESULT_CNT        =>  result_cnt,

      RESULT_CNT_UPDATE =>  result_cnt_update,
      CLK_CNT_UPDATE    =>  clk_cnt_update
   );

   DOUT_VLD <= validation;

   process(CLK) begin
      if (RST = '1') then
         DOUT <= "00000000";
         result_cnt <= "0000";
         clk_cnt <= "00000";
      elsif rising_edge(CLK) then
         if result_cnt_update ='0' then
            result_cnt <= "0000";
         end if;

         if (clk_cnt(4) = '1' and result_cnt_update = '1') then
            DOUT(to_integer(unsigned(result_cnt))) <= DIN;
            result_cnt <= result_cnt + 1;
            clk_cnt <= "00001";
         else 
            if clk_cnt_update = '0' then
               clk_cnt <= "00000";
            else 
               clk_cnt <= clk_cnt + 1;
            end if;
         end if;
      end if;
   end process;
   
end architecture behavioral;