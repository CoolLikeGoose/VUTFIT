-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2023 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): jmeno <login AT stud.fit.vutbr.cz>
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic;                      -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'

   -- stavove signaly
   READY    : out std_logic;                      -- hodnota 1 znamena, ze byl procesor inicializovan a zacina vykonavat program
   DONE     : out std_logic                       -- hodnota 1 znamena, ze procesor ukoncil vykonavani programu (narazil na instrukci halt)
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

-------------------types
type fsm_t is (
  s_init,
  s_reset,
  s_fetch,
  s_decode,
  s_i_pointer_next,   -- >
  s_i_pointer_prev,   -- <
  s_i_data_inc, s_data_inc_process,       -- +
  s_i_data_dec, s_data_dec_process,      -- -
  s_i_cycle_start,    -- [

  s_i_cycle_end,      -- ]
  s_cycle_check_start,    
  s_cycle_check_symb,     
  s_cycle_check_cnt,      

  s_i_cycle_break,    -- ~
  s_cycle_break_loop,   
  s_cycle_break_check,   

  s_i_data_write,     -- .
  s_i_data_read,      -- ,
  s_i_return,         -- @
  s_i_ignore          -- comments
);

signal cur_state      :   fsm_t;
signal next_state     :   fsm_t;

-----------------------------signals

--registers
signal ptr_reg_data   :   std_logic_vector(12 downto 0);
signal ptr_reg_inc    :   std_logic;
signal ptr_reg_dec    :   std_logic;

signal pc_reg_data    :   std_logic_vector(12 downto 0);
signal pc_reg_inc     :   std_logic;
signal pc_reg_dec     :   std_logic;

signal cnt_reg_data   :   std_logic_vector(7 downto 0); --max 255 cycles?
signal cnt_reg_equals0:   std_logic := '1';
signal cnt_reg_inc    :   std_logic;
signal cnt_reg_dec    :   std_logic;

--multiplexors
signal mx1_sel        :   std_logic;
signal mx2_sel        :   std_logic_vector(1 downto 0);

-- ----------------------------------------------------------------------------
--                            Implemetation
-- ----------------------------------------------------------------------------
begin
------------------------------registers
  PTR_reg: process (RESET, CLK, ptr_reg_inc, ptr_reg_dec)
  begin 
    if (RESET = '1') then 
      ptr_reg_data <= (others => '0');
    elsif rising_edge(CLK) then 
      if (ptr_reg_inc = '1') then 
        ptr_reg_data <= ptr_reg_data + 1;
      elsif (ptr_reg_dec = '1') then 
        ptr_reg_data <= ptr_reg_data - 1;
      end if;
    end if;
  end process PTR_reg;

  PC_reg: process (RESET, CLK, pc_reg_inc, pc_reg_dec)
  begin 
    if (RESET = '1') then 
      pc_reg_data <= (others => '0');
    elsif rising_edge(CLK) then 
      if (pc_reg_inc = '1') then 
        pc_reg_data <= pc_reg_data + 1;
      elsif (pc_reg_dec = '1') then 
        pc_reg_data <= pc_reg_data - 1;
      end if;
    end if;
  end process PC_reg;

  CNT_reg: process (RESET, CLK, cnt_reg_inc, cnt_reg_dec)
  begin 
    if (RESET = '1') then 
      cnt_reg_data <= (others => '0');
    elsif rising_edge(CLK) then 
      if (cnt_reg_inc = '1') then 
        cnt_reg_data <= cnt_reg_data + 1;
      elsif (cnt_reg_dec = '1') then 
        cnt_reg_data <= cnt_reg_data - 1;
      end if;
    end if;

    if (cnt_reg_data = "00000000") then 
      cnt_reg_equals0 <= '1';
    else 
      cnt_reg_equals0 <= '0';
    end if;
  end process CNT_reg;

---------------------------Multiplexors
  MX1: process (RESET, CLK, mx1_sel, ptr_reg_data, pc_reg_data) 
  begin
    if (mx1_sel = '0') then
      DATA_ADDR <= ptr_reg_data;
    else 
      DATA_ADDR <= pc_reg_data;
    end if;
  end process MX1;
  
  MX2: process (RESET, CLK, mx2_sel, IN_DATA, DATA_RDATA) 
  begin
    case (mx2_sel) is
      when "00" =>  DATA_WDATA <= IN_DATA;
      when "01" =>  DATA_WDATA <= DATA_RDATA - 1;
      when "10" =>  DATA_WDATA <= DATA_RDATA + 1;
      when others => DATA_WDATA <= (others => '0');
    end case;
  end process MX2;
  
  -----------------------------FSM
  fsm_support: process(CLK, RESET)
  begin
    if (RESET = '1') then
      READY <= '0';
      cur_state <= s_reset;
    end if;

    if (next_state = s_fetch) then 
      READY <= '1';
    end if;

    if (rising_edge(CLK) and EN = '1') then
      cur_state <= next_state; --trigger fsm
    end if;
  end process fsm_support;

  fsm: process(CLK, RESET, cur_state)
  begin     
      mx1_sel <= '0';
      mx2_sel <= (others => '0');

      DONE <= '0';
      ptr_reg_inc   <=  '0';
      ptr_reg_dec   <=  '0';
      pc_reg_inc    <=  '0';
      pc_reg_dec    <=  '0';
      cnt_reg_inc   <=  '0';
      cnt_reg_dec   <=  '0';

      DATA_RDWR     <=  '0';
      DATA_EN       <=  '0';
      OUT_WE        <=  '0';
      IN_REQ        <=  '0';
      OUT_DATA      <=  (others => '0');

      case (cur_state) is

        when s_reset => 
          DATA_EN <= '1';
          next_state <= s_init;

        when s_init => 
          DATA_EN <= '1'; 
          if (DATA_RDATA = X"40") then
            next_state <= s_fetch;
            -- ptr_reg_inc   <=  '1';
          else 
            ptr_reg_inc   <=  '1';
          end if;

        when s_fetch => 
          mx1_sel <= '1';
          DATA_EN <= '1'; 
          next_state <= s_decode;

        when s_decode =>
          case (DATA_RDATA) is 
            when X"3E"  => next_state <= s_i_pointer_next;
            when X"3C"  => next_state <= s_i_pointer_prev;
            when X"2B"  => next_state <= s_i_data_inc;
            when X"2D"  => next_state <= s_i_data_dec;
            when X"5B"  => next_state <= s_i_cycle_start;
            when X"5D"  => next_state <= s_i_cycle_end;
            when X"7E"  => next_state <= s_i_cycle_break;
            when X"2E"  => next_state <= s_i_data_write;
            when X"2C"  => next_state <= s_i_data_read;
            when X"40"  => next_state <= s_i_return;
            when others => next_state <= s_i_ignore;
          end case;
          DATA_EN <= '1';

        when s_i_pointer_next =>
          ptr_reg_inc <= '1';
          pc_reg_inc  <= '1';
          next_state <= s_fetch;

        when s_i_pointer_prev => 
          ptr_reg_dec <= '1';
          pc_reg_inc <= '1';
          next_state <= s_fetch;  

        when s_i_data_inc =>
          DATA_RDWR <= '1';
          DATA_EN <= '1';
          pc_reg_inc <= '1';
          mx2_sel <= "10";
          next_state <= s_fetch;
            

        when s_i_data_dec =>
          DATA_RDWR <= '1';
          DATA_EN <= '1';
          pc_reg_inc <= '1';
          mx2_sel <= "01";
          next_state <= s_fetch;
            

        when s_i_cycle_start =>
          if (DATA_RDATA = "00000000") then   --skip
            next_state <= s_i_cycle_break;
          else 
            pc_reg_inc <= '1';
            next_state <= s_fetch;            --process while
          end if;
        
        when s_i_cycle_end =>
          if (DATA_RDATA = "00000000") then   --next while
            pc_reg_inc <= '1';
            next_state <= s_fetch;
          else
            cnt_reg_inc <= '1'; 
            pc_reg_dec <= '1';                --go back
            next_state <= s_cycle_check_start;
          end if;
        
        when s_cycle_check_start =>
          if (cnt_reg_equals0 = '1') then     --check for start cycle
            next_state <= s_fetch;
          else 
            DATA_EN <= '1';                   --Get code
            mx1_sel <= '1';
            next_state <= s_cycle_check_symb;
          end if;
        
        when s_cycle_check_symb =>
          next_state <= s_cycle_check_cnt;
          if (DATA_RDATA = X"5B") then 
            cnt_reg_dec <= '1';
          elsif (DATA_RDATA = X"5D") then 
            cnt_reg_inc <= '1';
          end if;

        when s_cycle_check_cnt => 
          next_state <= s_cycle_check_start;
          if (cnt_reg_equals0 = '0') then 
            pc_reg_dec <= '1';
          end if;
        
        when s_i_cycle_break =>
          pc_reg_inc <= '1';                --go forward
          cnt_reg_inc <= '1';
          next_state <= s_cycle_break_loop;
        
        when s_cycle_break_loop =>
          if (cnt_reg_equals0 = '1') then   --end of cycle founded
            next_state <= s_fetch;
          else 
            mx1_sel <= '1';                 --get next code symb
            DATA_EN <= '1';
            next_state <= s_cycle_break_check;
          end if;
        
        when s_cycle_break_check => 
          pc_reg_inc <= '1';
          next_state <= s_cycle_break_loop;
          if (DATA_RDATA = X"5B") then      --check for next code symb 
            cnt_reg_inc <= '1';             --and update counter
          elsif (DATA_RDATA = X"5D") then 
            cnt_reg_dec <= '1';
          end if;

        when s_i_data_write =>
            if (OUT_BUSY = '1') then
              next_state <= s_i_data_write;
            else
              OUT_DATA <= DATA_RDATA;
              OUT_WE <= '1';
              pc_reg_inc <= '1';
              next_state <= s_fetch;
            end if;

        when s_i_data_read =>
              if (IN_VLD = '1') then 
                DATA_EN <= '1';
                DATA_RDWR <= '1';
                pc_reg_inc <= '1';
                next_state <= s_fetch;
              else 
                IN_REQ <= '1';
                next_state <= s_i_data_read;
              end if;
            
        when s_i_return => DONE <= '1';
        when s_i_ignore => 
          next_state <= s_fetch;
          pc_reg_inc <= '1';

        when others => null;
      end case;
  end process fsm; 
end behavioral;

