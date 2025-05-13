library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
entity UART_RX is
  port (
    CLK: in std_logic;
    RST: in std_logic;
    DIN: in std_logic;
    DOUT: out std_logic_vector (7 downto 0);
    DOUT_VLD: out std_logic
  );
end UART_RX;
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart_rx_fsm is
  port (
    clk : in std_logic;
    rst : in std_logic;
    din : in std_logic;
    clk_cnt : in std_logic_vector (4 downto 0);
    result_cnt : in std_logic_vector (3 downto 0);
    dout_vld : out std_logic;
    result_cnt_update : out std_logic;
    clk_cnt_update : out std_logic);
end entity uart_rx_fsm;

architecture rtl of uart_rx_fsm is
  signal current_state : std_logic_vector (2 downto 0);
  signal n91_o : std_logic;
  signal n93_o : std_logic_vector (2 downto 0);
  signal n95_o : std_logic;
  signal n97_o : std_logic;
  signal n99_o : std_logic_vector (2 downto 0);
  signal n101_o : std_logic;
  signal n103_o : std_logic;
  signal n105_o : std_logic_vector (2 downto 0);
  signal n107_o : std_logic;
  signal n109_o : std_logic;
  signal n111_o : std_logic_vector (2 downto 0);
  signal n113_o : std_logic;
  signal n115_o : std_logic;
  signal n116_o : std_logic_vector (4 downto 0);
  signal n120_o : std_logic;
  signal n125_o : std_logic;
  signal n130_o : std_logic;
  signal n133_o : std_logic_vector (2 downto 0);
  signal n134_o : std_logic;
  signal n135_o : std_logic;
  signal n136_o : std_logic;
  signal n138_o : std_logic_vector (2 downto 0);
  signal n144_q : std_logic;
  signal n145_q : std_logic;
  signal n146_q : std_logic;
  signal n147_q : std_logic_vector (2 downto 0) := "000";
begin
  dout_vld <= n144_q;
  result_cnt_update <= n145_q;
  clk_cnt_update <= n146_q;
  -- work/uart_rx_fsm.vhd:26:11
  current_state <= n147_q; -- (isignal)
  -- work/uart_rx_fsm.vhd:40:26
  n91_o <= not din;
  -- work/uart_rx_fsm.vhd:40:19
  n93_o <= current_state when n91_o = '0' else "001";
  -- work/uart_rx_fsm.vhd:35:16
  n95_o <= '1' when current_state = "000" else '0';
  -- work/uart_rx_fsm.vhd:47:30
  n97_o <= '1' when clk_cnt = "10011" else '0';
  -- work/uart_rx_fsm.vhd:47:19
  n99_o <= current_state when n97_o = '0' else "010";
  -- work/uart_rx_fsm.vhd:44:16
  n101_o <= '1' when current_state = "001" else '0';
  -- work/uart_rx_fsm.vhd:54:33
  n103_o <= '1' when result_cnt = "1000" else '0';
  -- work/uart_rx_fsm.vhd:54:19
  n105_o <= current_state when n103_o = '0' else "011";
  -- work/uart_rx_fsm.vhd:51:16
  n107_o <= '1' when current_state = "010" else '0';
  -- work/uart_rx_fsm.vhd:61:30
  n109_o <= '1' when clk_cnt = "10000" else '0';
  -- work/uart_rx_fsm.vhd:61:19
  n111_o <= current_state when n109_o = '0' else "100";
  -- work/uart_rx_fsm.vhd:58:16
  n113_o <= '1' when current_state = "011" else '0';
  -- work/uart_rx_fsm.vhd:65:16
  n115_o <= '1' when current_state = "100" else '0';
  n116_o <= n115_o & n113_o & n107_o & n101_o & n95_o;
  -- work/uart_rx_fsm.vhd:34:13
  with n116_o select n120_o <=
    '1' when "10000",
    n144_q when "01000",
    n144_q when "00100",
    n144_q when "00010",
    '0' when "00001",
    'X' when others;
  -- work/uart_rx_fsm.vhd:34:13
  with n116_o select n125_o <=
    n145_q when "10000",
    '0' when "01000",
    '1' when "00100",
    n145_q when "00010",
    '0' when "00001",
    'X' when others;
  -- work/uart_rx_fsm.vhd:34:13
  with n116_o select n130_o <=
    '0' when "10000",
    n146_q when "01000",
    n146_q when "00100",
    '1' when "00010",
    '0' when "00001",
    'X' when others;
  -- work/uart_rx_fsm.vhd:34:13
  with n116_o select n133_o <=
    "000" when "10000",
    n111_o when "01000",
    n105_o when "00100",
    n99_o when "00010",
    n93_o when "00001",
    "XXX" when others;
  -- work/uart_rx_fsm.vhd:31:10
  n134_o <= n120_o when rst = '0' else n144_q;
  -- work/uart_rx_fsm.vhd:31:10
  n135_o <= n125_o when rst = '0' else n145_q;
  -- work/uart_rx_fsm.vhd:31:10
  n136_o <= n130_o when rst = '0' else n146_q;
  -- work/uart_rx_fsm.vhd:31:10
  n138_o <= n133_o when rst = '0' else "000";
  -- work/uart_rx_fsm.vhd:30:7
  process (clk)
  begin
    if rising_edge (clk) then
      n144_q <= n134_o;
    end if;
  end process;
  -- work/uart_rx_fsm.vhd:30:7
  process (clk)
  begin
    if rising_edge (clk) then
      n145_q <= n135_o;
    end if;
  end process;
  -- work/uart_rx_fsm.vhd:30:7
  process (clk)
  begin
    if rising_edge (clk) then
      n146_q <= n136_o;
    end if;
  end process;
  -- work/uart_rx_fsm.vhd:30:7
  process (clk)
  begin
    if rising_edge (clk) then
      n147_q <= n138_o;
    end if;
  end process;
end rtl;


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

architecture rtl of uart_rx is
  signal wrap_CLK: std_logic;
  signal wrap_RST: std_logic;
  signal wrap_DIN: std_logic;
  subtype typwrap_DOUT is std_logic_vector (7 downto 0);
  signal wrap_DOUT: typwrap_DOUT;
  signal wrap_DOUT_VLD: std_logic;
  signal validation : std_logic;
  signal clk_cnt : std_logic_vector (4 downto 0);
  signal result_cnt : std_logic_vector (3 downto 0);
  signal result_cnt_update : std_logic;
  signal clk_cnt_update : std_logic;
  signal fsm_dout_vld : std_logic;
  signal fsm_result_cnt_update : std_logic;
  signal fsm_clk_cnt_update : std_logic;
  signal n8_o : std_logic;
  signal n10_o : std_logic_vector (3 downto 0);
  signal n11_o : std_logic;
  signal n12_o : std_logic;
  signal n14_o : std_logic_vector (2 downto 0);
  signal n18_o : std_logic_vector (3 downto 0);
  signal n19_o : std_logic;
  signal n21_o : std_logic_vector (4 downto 0);
  signal n23_o : std_logic_vector (4 downto 0);
  signal n26_o : std_logic_vector (4 downto 0);
  signal n27_o : std_logic_vector (3 downto 0);
  signal n38_q : std_logic_vector (7 downto 0);
  signal n39_q : std_logic_vector (4 downto 0);
  signal n40_q : std_logic_vector (3 downto 0);
  signal n41_o : std_logic;
  signal n42_o : std_logic;
  signal n43_o : std_logic;
  signal n44_o : std_logic;
  signal n45_o : std_logic;
  signal n46_o : std_logic;
  signal n47_o : std_logic;
  signal n48_o : std_logic;
  signal n49_o : std_logic;
  signal n50_o : std_logic;
  signal n51_o : std_logic;
  signal n52_o : std_logic;
  signal n53_o : std_logic;
  signal n54_o : std_logic;
  signal n55_o : std_logic;
  signal n56_o : std_logic;
  signal n57_o : std_logic;
  signal n58_o : std_logic;
  signal n59_o : std_logic;
  signal n60_o : std_logic;
  signal n61_o : std_logic;
  signal n62_o : std_logic;
  signal n63_o : std_logic;
  signal n64_o : std_logic;
  signal n65_o : std_logic;
  signal n66_o : std_logic;
  signal n67_o : std_logic;
  signal n68_o : std_logic;
  signal n69_o : std_logic;
  signal n70_o : std_logic;
  signal n71_o : std_logic;
  signal n72_o : std_logic;
  signal n73_o : std_logic;
  signal n74_o : std_logic;
  signal n75_o : std_logic;
  signal n76_o : std_logic;
  signal n77_o : std_logic;
  signal n78_o : std_logic;
  signal n79_o : std_logic;
  signal n80_o : std_logic;
  signal n81_o : std_logic;
  signal n82_o : std_logic;
  signal n83_o : std_logic_vector (7 downto 0);
begin
  wrap_clk <= clk;
  wrap_rst <= rst;
  wrap_din <= din;
  dout <= wrap_dout;
  dout_vld <= wrap_dout_vld;
  wrap_DOUT <= n38_q;
  wrap_DOUT_VLD <= validation;
  -- work/uart_rx.vhd:22:12
  validation <= fsm_dout_vld; -- (signal)
  -- work/uart_rx.vhd:23:12
  clk_cnt <= n39_q; -- (signal)
  -- work/uart_rx.vhd:24:12
  result_cnt <= n40_q; -- (signal)
  -- work/uart_rx.vhd:25:12
  result_cnt_update <= fsm_result_cnt_update; -- (signal)
  -- work/uart_rx.vhd:26:12
  clk_cnt_update <= fsm_clk_cnt_update; -- (signal)
  -- work/uart_rx.vhd:30:4
  fsm : entity work.uart_rx_fsm port map (
    clk => wrap_CLK,
    rst => wrap_RST,
    din => wrap_DIN,
    clk_cnt => clk_cnt,
    result_cnt => result_cnt,
    dout_vld => fsm_dout_vld,
    result_cnt_update => fsm_result_cnt_update,
    clk_cnt_update => fsm_clk_cnt_update);
  -- work/uart_rx.vhd:53:31
  n8_o <= not result_cnt_update;
  -- work/uart_rx.vhd:53:10
  n10_o <= result_cnt when n8_o = '0' else "0000";
  -- work/uart_rx.vhd:57:21
  n11_o <= clk_cnt (4);
  -- work/uart_rx.vhd:57:31
  n12_o <= n11_o and result_cnt_update;
  -- work/uart_rx.vhd:58:17
  n14_o <= result_cnt (2 downto 0);  --  trunc
  -- work/uart_rx.vhd:59:38
  n18_o <= std_logic_vector (unsigned (result_cnt) + unsigned'("0001"));
  -- work/uart_rx.vhd:62:31
  n19_o <= not clk_cnt_update;
  -- work/uart_rx.vhd:65:35
  n21_o <= std_logic_vector (unsigned (clk_cnt) + unsigned'("00001"));
  -- work/uart_rx.vhd:62:13
  n23_o <= n21_o when n19_o = '0' else "00000";
  -- work/uart_rx.vhd:57:10
  n26_o <= n23_o when n12_o = '0' else "00001";
  -- work/uart_rx.vhd:57:10
  n27_o <= n10_o when n12_o = '0' else n18_o;
  -- work/uart_rx.vhd:52:7
  process (wrap_CLK, wrap_RST)
  begin
    if wrap_RST = '1' then
      n38_q <= "00000000";
    elsif rising_edge (wrap_CLK) then
      n38_q <= n83_o;
    end if;
  end process;
  -- work/uart_rx.vhd:52:7
  process (wrap_CLK, wrap_RST)
  begin
    if wrap_RST = '1' then
      n39_q <= "00000";
    elsif rising_edge (wrap_CLK) then
      n39_q <= n26_o;
    end if;
  end process;
  -- work/uart_rx.vhd:52:7
  process (wrap_CLK, wrap_RST)
  begin
    if wrap_RST = '1' then
      n40_q <= "0000";
    elsif rising_edge (wrap_CLK) then
      n40_q <= n27_o;
    end if;
  end process;
  -- work/uart_rx.vhd:57:10
  n41_o <= n14_o (2);
  -- work/uart_rx.vhd:57:10
  n42_o <= not n41_o;
  -- work/uart_rx.vhd:57:10
  n43_o <= n14_o (1);
  -- work/uart_rx.vhd:57:10
  n44_o <= not n43_o;
  -- work/uart_rx.vhd:57:10
  n45_o <= n42_o and n44_o;
  -- work/uart_rx.vhd:57:10
  n46_o <= n42_o and n43_o;
  -- work/uart_rx.vhd:57:10
  n47_o <= n41_o and n44_o;
  -- work/uart_rx.vhd:57:10
  n48_o <= n41_o and n43_o;
  -- work/uart_rx.vhd:57:10
  n49_o <= n14_o (0);
  -- work/uart_rx.vhd:57:10
  n50_o <= not n49_o;
  -- work/uart_rx.vhd:57:10
  n51_o <= n45_o and n50_o;
  -- work/uart_rx.vhd:57:10
  n52_o <= n45_o and n49_o;
  -- work/uart_rx.vhd:57:10
  n53_o <= n46_o and n50_o;
  -- work/uart_rx.vhd:57:10
  n54_o <= n46_o and n49_o;
  -- work/uart_rx.vhd:57:10
  n55_o <= n47_o and n50_o;
  -- work/uart_rx.vhd:57:10
  n56_o <= n47_o and n49_o;
  -- work/uart_rx.vhd:57:10
  n57_o <= n48_o and n50_o;
  -- work/uart_rx.vhd:57:10
  n58_o <= n48_o and n49_o;
  n59_o <= n38_q (0);
  -- work/uart_rx.vhd:57:10
  n60_o <= n51_o and n12_o;
  -- work/uart_rx.vhd:57:10
  n61_o <= n59_o when n60_o = '0' else wrap_DIN;
  n62_o <= n38_q (1);
  -- work/uart_rx.vhd:57:10
  n63_o <= n52_o and n12_o;
  -- work/uart_rx.vhd:57:10
  n64_o <= n62_o when n63_o = '0' else wrap_DIN;
  n65_o <= n38_q (2);
  -- work/uart_rx.vhd:57:10
  n66_o <= n53_o and n12_o;
  -- work/uart_rx.vhd:57:10
  n67_o <= n65_o when n66_o = '0' else wrap_DIN;
  n68_o <= n38_q (3);
  -- work/uart_rx.vhd:57:10
  n69_o <= n54_o and n12_o;
  -- work/uart_rx.vhd:57:10
  n70_o <= n68_o when n69_o = '0' else wrap_DIN;
  n71_o <= n38_q (4);
  -- work/uart_rx.vhd:57:10
  n72_o <= n55_o and n12_o;
  -- work/uart_rx.vhd:57:10
  n73_o <= n71_o when n72_o = '0' else wrap_DIN;
  n74_o <= n38_q (5);
  -- work/uart_rx.vhd:57:10
  n75_o <= n56_o and n12_o;
  -- work/uart_rx.vhd:57:10
  n76_o <= n74_o when n75_o = '0' else wrap_DIN;
  n77_o <= n38_q (6);
  -- work/uart_rx.vhd:57:10
  n78_o <= n57_o and n12_o;
  -- work/uart_rx.vhd:57:10
  n79_o <= n77_o when n78_o = '0' else wrap_DIN;
  n80_o <= n38_q (7);
  -- work/uart_rx.vhd:57:10
  n81_o <= n58_o and n12_o;
  -- work/uart_rx.vhd:57:10
  n82_o <= n80_o when n81_o = '0' else wrap_DIN;
  n83_o <= n82_o & n79_o & n76_o & n73_o & n70_o & n67_o & n64_o & n61_o;
end rtl;
