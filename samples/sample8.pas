{Test na cislicove literaly}
var
	a : integer;
	b : integer;
	c : integer;
	d : integer;
	vysl : integer;
begin
	a := 0010;
	write('a '#10);
	b := 000010.00004;
	write('b '#10);
	c := 0010e-4;
	write('c '#10);	
	d := 0010e+4;
	write('d '#10);
end.
