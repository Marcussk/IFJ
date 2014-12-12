{ find basic test }
var
	res : integer;
	res2 : integer;
begin
res := find('fdsgsadas'#10'', 'sgs');
write(res, ''#10'');
res2 := find('Honzikova cesta', 'cesta');
write(res2, ''#10'');

res2 := find('Honzikova cesta'#10'do mordoru', 'Honzikova cesta'#10'do mordoru');
write(res2, ''#10'');

res2 := find('Honzikova cesta'#10'do mordoru', 'Honzik');
write(res2, ''#10'');

res2 := find('Honzikova cesta', 'cesta do pekla');
write(res2, ''#10'')

end.
