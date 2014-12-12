{ Jednoduchy test na write }
var
	a : integer;
	b : integer;
	c : string;
	d : real;
begin
	c := 'Ultra hello world'#10'';
	a :=10 + 20;
	d :=118e-5;
	write(12345);
	write(''#10'');
	write('Hello world'#10'');
	write(a);
	a := a*2;
	write(''#10'');
	write(a, 'Hello 2', 99, ''#10'');
	write(c, ''#10'');
	write(d, ''#10'')
end.
