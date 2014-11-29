{ Jednoduchy test na write }
var
	a : integer;
	b : integer;
	c : string;
begin
	c := 'Ultra hello world'#10'';
	a :=10 + 20;
	write(12345);
	write(''#10'');
	write('Hello world'#10'');
	write(a);
	c := c*2;
	write(''#10'');
	write(c)
end.
