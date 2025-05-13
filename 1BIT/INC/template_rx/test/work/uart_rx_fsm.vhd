-- -- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- -- Author(s): Nikita Smirnov (xsmirn02)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity UART_RX_FSM is
   port(
      CLK               :  in std_logic;
      RST               :  in std_logic;
         
      DIN               :  in std_logic;     
      DOUT_VLD          :  out std_logic;
      
      CLK_CNT           :  in std_logic_vector(4 downto 0); 
      RESULT_CNT        :  in std_logic_vector(3 downto 0);

      RESULT_CNT_UPDATE :  out std_logic;
      CLK_CNT_UPDATE    :  out std_logic
   );
end entity UART_RX_FSM;

architecture behavioral of UART_RX_FSM is
   type STATE is (START_BIT_WAIT, MID_BIT_WAIT, RECEIVING, STOP_BIT_WAIT, VALIDATED);
   signal current_state : STATE := START_BIT_WAIT; 

begin
   process(CLK) begin
      if rising_edge(CLK) then
         if RST = '1' then
            current_state <= START_BIT_WAIT;
         else 
            case current_state is
               when START_BIT_WAIT =>
                  DOUT_VLD <= '0';
                  RESULT_CNT_UPDATE <= '0';
                  CLK_CNT_UPDATE <= '0';

                  if DIN = '0' then
                     current_state <= MID_BIT_WAIT;
                  end if;
                  
               when MID_BIT_WAIT =>
                  CLK_CNT_UPDATE <= '1';

                  if CLK_CNT = "10011" then -- skip 19 (midbit) (P.S. 5 cyklu se ztratilo ¯\_(ツ)_/¯)
                  current_state <= RECEIVING;
                  end if;
                  
               when RECEIVING =>
                  RESULT_CNT_UPDATE <= '1';

                  if RESULT_CNT = "1000" then
                     current_state <= STOP_BIT_WAIT;
                  end if;
                  
               when STOP_BIT_WAIT =>
                  RESULT_CNT_UPDATE <= '0';

                  if CLK_CNT = "10000" then
                     current_state <= VALIDATED;
                  end if;
                  
               when VALIDATED =>
                  DOUT_VLD <= '1';
                  CLK_CNT_UPDATE <= '0';

                  current_state <= START_BIT_WAIT;
            end case;
         end if;
      end if;
   end process;
end architecture behavioral;