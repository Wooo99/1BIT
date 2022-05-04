-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST_DEFAULT, TEST_WRONG, TEST_1, TEST_15, TEST_159, TEST_1598, TEST_15989, TEST_159890, TEST_1598908, TEST_15989083, TEST_159890831,	TEST_1598908316, TEST_15989083168, TEST_159899, TEST_1598994, TEST_15989943, TEST_159899438, TEST_1598994389,	PRINT_ACCESS_ALLOWED, PRINT_ACCESS_DENIED, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST_DEFAULT;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_DEFAULT =>
      next_state <= TEST_DEFAULT;
		if (KEY(1) = '1') then
         next_state <= TEST_1;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	 when TEST_1 =>
      next_state <= TEST_1;
		if (KEY(5) = '1') then
         next_state <= TEST_15;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_15 =>
      next_state <= TEST_15;
		if (KEY(9) = '1') then
         next_state <= TEST_159;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_159 =>
      next_state <= TEST_159;
		if (KEY(8) = '1') then
         next_state <= TEST_1598;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_1598 =>
      next_state <= TEST_1598;
		if (KEY(9) = '1') then
         next_state <= TEST_15989;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_15989 =>
      next_state <= TEST_15989;
		if (KEY(0) = '1') then
         next_state <= TEST_159890;
		elsif (KEY(9) = '1') then
         next_state <= TEST_159899;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_159899 =>
      next_state <= TEST_159899;
		if (KEY(4) = '1') then
         next_state <= TEST_1598994;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_1598994 =>
      next_state <= TEST_1598994;
		if (KEY(3) = '1') then
         next_state <= TEST_15989943;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_15989943 =>
      next_state <= TEST_15989943;
		if (KEY(8) = '1') then
         next_state <= TEST_159899438;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_159899438 =>
      next_state <= TEST_159899438;
		if (KEY(9) = '1') then
         next_state <= TEST_1598994389;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_1598994389 =>
      next_state <= TEST_1598994389;
      if (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_ALLOWED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_159890 =>
      next_state <= TEST_159890;
		if (KEY(8) = '1') then
         next_state <= TEST_1598908;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_1598908 =>
      next_state <= TEST_1598908;
		if (KEY(3) = '1') then
         next_state <= TEST_15989083;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_15989083 =>
      next_state <= TEST_15989083;
		if (KEY(1) = '1') then
         next_state <= TEST_159890831;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_159890831 =>
      next_state <= TEST_159890831;
		if (KEY(6) = '1') then
         next_state <= TEST_1598908316;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_1598908316 =>
      next_state <= TEST_1598908316;
		if (KEY(8) = '1') then
         next_state <= TEST_15989083168;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_15989083168 =>
      next_state <= TEST_15989083168;
      if (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_ALLOWED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_WRONG =>
		next_state <= TEST_WRONG;
      if (KEY(15) = '1') then
         next_state <= PRINT_ACCESS_DENIED;
			elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= TEST_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ACCESS_DENIED =>
      next_state <= PRINT_ACCESS_DENIED;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when PRINT_ACCESS_ALLOWED =>
      next_state <= PRINT_ACCESS_ALLOWED;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST_DEFAULT; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST_DEFAULT;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_DEFAULT |TEST_WRONG | TEST_1 | TEST_15 | TEST_159 | TEST_1598 | TEST_15989 |TEST_159899 |TEST_1598994 |TEST_15989943 |TEST_159899438 |TEST_1598994389 | TEST_159890 | TEST_1598908 | TEST_15989083 | TEST_159890831 | TEST_1598908316 | TEST_15989083168 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ACCESS_DENIED =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
	   when PRINT_ACCESS_ALLOWED =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   end case;
end process output_logic;

end architecture behavioral;

