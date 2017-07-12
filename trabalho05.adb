with Ada.Real_Time; use Ada.Real_Time;
with Text_IO; use Text_IO;
procedure trabalho05 is

canal: array(1..6) of Integer:=(-1,-1,-1,-1,-1,-1);
type vetor is array(1..3) of Integer;
comparacao: vetor;

-- função de envio assíncrono
procedure send (buf: in Integer; c: in Integer) is
begin
  canal(c) := buf; -- valor do buffer é copiado na posição c do vetor canal
  --Put_Line("send" & Integer'Image(canal(c)));
end send;

-- função de recebimento
procedure receive (buf: out Integer; c: in Integer) is
begin
   while canal(c) = -1 loop
        null;
     end loop;
     buf:=canal(c);
     --Put_Line("receive" & Integer'Image(buf));
     canal(c):=-1;
end receive;

task type threadA;
 task body threadA is
   voto : Integer := 10; -- mudar voto para obter saída diferente
   status : Integer;
 begin
   send(voto,1); -- primeiro canal para send
   receive(status,4); -- primeiro canal para receive
   --Put_Line("status A:" & Integer'Image(status));
   if status = 0 then -- status de falha
     Put_Line("A versao A falhou. Finalizando."); -- mostra e depois finaliza
   else
     Put_Line("A versao A continua executando."); -- continua executando
     while true loop
       null;
     end loop;
   end if;
end threadA;

task type threadB;
 task body threadB is
   voto : Integer := 10; -- mudar voto para obter saída diferente
   status : Integer;
 begin
   send(voto,2); -- segundo canal para send
   receive(status,5); -- segundo canal para receive
   --Put_Line("status B:" & Integer'Image(status));
   if status = 0 then -- status de falha
     Put_Line("A versao B falhou. Finalizando."); -- mostra e depois finaliza
   else
     Put_Line("A versao B continua executando."); -- continua executando
     while true loop
       null;
     end loop;
   end if;
end threadB;

task type threadC;
 task body threadC is
   voto : Integer := 10; -- mudar voto para obter saída diferente
   status : Integer;
 begin
   send(voto,3); -- terceiro canal para send
   receive(status,6); -- teceiro canal para receive
   --Put_Line("status C:" & Integer'Image(status));
   if status = 0 then -- status de falha
     Put_Line("A versao C falhou. Finalizando."); -- mostra e depois finaliza
   else
     Put_Line("A versao C continua executando."); -- continua executando
     while true loop
       null;
     end loop;
   end if;
end threadC;

-- função de compraração dos votos
function compara(comparacao: in vetor) return Integer is
  versaoErrada: Integer;
  begin
     if comparacao(1) = comparacao(2) and comparacao(2) = comparacao(3) then -- a = b = c
     Put_Line("Nenhuma versao falhou.");
     Put_Line("Voto majoritario:" & Integer'Image(comparacao(1)));
     versaoErrada := 0; -- todas são iguais
   elsif comparacao(1) = comparacao(2) then -- somente a = b
     Put_Line("Versao C falhou.");
     Put_Line("Voto majoritario:" & Integer'Image(comparacao(1)));
     Put_Line("Voto minoritario:" & Integer'Image(comparacao(3)));
     versaoErrada := 3; -- c é diferente
   elsif comparacao(2) = comparacao(3) then -- somente b = c
     Put_Line("Versao A falhou.");
     Put_Line("Voto majoritario:" & Integer'Image(comparacao(2)));
     Put_Line("Voto minoritario:" & Integer'Image(comparacao(1)));
     versaoErrada := 1; -- a é diferente
   else -- somente a = c
     Put_Line("Versao B falhou.");
     Put_Line("Voto majoritario:" & Integer'Image(comparacao(1)));
     Put_Line("Voto minoritario:" & Integer'Image(comparacao(2)));
     versaoErrada := 2; -- b é diferente
   end if;
    return versaoErrada;
  end compara;


task type threadDriver;
 task body threadDriver is
   versaoErrada: Integer;
 begin
   receive(comparacao(1),1);
   --Put_Line("driver - A:" & Integer'Image(comparacao(1)));
   receive(comparacao(2),2);
   --Put_Line("driver - B:" & Integer'Image(comparacao(2)));
   receive(comparacao(3),3);
   --Put_Line("driver - C:" & Integer'Image(comparacao(3)));
   
   versaoErrada := compara(comparacao);

   if versaoErrada = 0 then -- todas ok
     send(1,4); -- A
     send(1,5); -- B
     send(1,6); -- C
   elsif versaoErrada = 1 then -- A falhou
     send(0,4); -- A
     send(1,5); -- B
     send(1,6); -- C
   elsif versaoErrada = 2 then -- B falhou
     send(1,4); -- A
     send(0,5); -- B
     send(1,6); -- C
   else -- C falhou
     send(1,4); -- A
     send(1,5); -- B
     send(0,6); -- C
   end if;  
end threadDriver;

-- inicialização de threads
A : threadA;
B : threadB;
C : threadC;
D : threadDriver;

begin
  null;
end trabalho05;
