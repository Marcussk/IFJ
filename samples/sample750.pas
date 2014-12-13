var
	a : integer;
	text : string;

begin
	while a <= 10 do
	begin
		write(a, ''#10'');
		a := a + 1
	end;

	write(''#10'');

	readln(text);

	vypis_textu();
end.

function vypis_textu(str : string) : integer;
	var
		c : integer;

begin
	c := 20 * 3;

	if str = 'Hello world!' then
	begin
		write(str);
	end
	else
	begin
		write('Nespravny text');
	end

	write(c);
end;