% Parser function for Gaussian logs.
%
% g.t.p.charnock@durham.ac.uk
% ilya.kuprov@durham.ac.uk

function props=g03_parse(filename)

g03_output=textread(filename, '%s','delimiter','\n');
for n=1:length(g03_output)
   
   %% Read the input orientation and atomic numbers
   if strcmp(deblank(g03_output(n)),'Input orientation:')==1
      k=n+5; m=1;
      while strcmp(deblank(g03_output(k)),'---------------------------------------------------------------------')~=1
         S=eval(['[' char(g03_output(k)) ']']); atoms(m,:)=S(4:6); atomic_numbers(m)=S(2); k=k+1; m=m+1; %#ok<AGROW>
      end
      natoms=m-1; props.inp_geom=atoms; props.natoms=natoms;
   end

   %% Read the standard orientation and atomic numbers
   if strcmp(deblank(g03_output(n)),'Standard orientation:')==1
      k=n+5; m=1;
      while strcmp(deblank(g03_output(k)),'---------------------------------------------------------------------')~=1
         S=eval(['[' char(g03_output(k)) ']']); atoms(m,:)=S(4:6); atomic_numbers(m)=S(2); k=k+1; m=m+1; %#ok<AGROW>
      end
      natoms=m-1; props.std_geom=atoms; props.natoms=natoms;
   end

   %% Read the SCF energy
   current_line=char(deblank(g03_output(n)));
   if length(current_line)>10 && strcmp(current_line(1:9),'SCF Done:')==1
       props.energy=str2double(current_line(26:41));
   end

   %% Read the isotropic hyperfine couplings
   if strcmp(deblank(g03_output(n)),'Isotropic Fermi Contact Couplings')==1
      props.hfc.iso=zeros(natoms,1);
      for k=1:natoms
         S=char(g03_output(n+1+k)); S=eval(['[' S(20:end) ']']); props.hfc.iso(k)=S(3);
      end          
   end

   %% Read the anisotropic hyperfine couplings
   if strcmp(deblank(g03_output(n)),'Anisotropic Spin Dipole Couplings in Principal Axis System')==1
      props.hfc.full.eigvals=cell(natoms,1);
      props.hfc.full.eigvecs=cell(natoms,1);
      props.hfc.full.matrix=cell(natoms,1);
      for k=1:natoms
         baa=g03_output(n+4*k+1); baa=char(baa); baa=eval(['[' baa(4:end) ']']);
         bbb=g03_output(n+4*k+2); bbb=char(bbb); bbb=eval(['[' bbb(14:end) ']']);
         bcc=g03_output(n+4*k+3); bcc=char(bcc); bcc=eval(['[' bcc(4:end) ']']);
         props.hfc.full.eigvals{k}=[baa(3) bbb(3) bcc(3)]+props.hfc.iso(k);
         props.hfc.full.eigvecs{k}=[baa(5:7)' bbb(5:7)' bcc(5:7)'];
         props.hfc.full.matrix{k}=props.hfc.full.eigvecs{k}'*diag(props.hfc.full.eigvals{k})*props.hfc.full.eigvecs{k};
      end
   end

   %% Read the g-tensor
   if strcmp(deblank(g03_output(n)),'g tensor [g = g_e + g_RMC + g_DC + g_OZ/SOC]:')
      line1=char(deblank(g03_output(n+1))); line2=char(deblank(g03_output(n+2))); line3=char(deblank(g03_output(n+3)));
      g=eval(['[' line1(5:18) '   ' line1(26:39) '   ' line1(46:60) ';   '...
                  line2(5:18) '   ' line2(26:39) '   ' line2(46:60) ';   '...
                  line3(5:18) '   ' line3(26:39) '   ' line3(46:60) ']']);
      g=(g+g')/2; [V,D]=eig(g); props.g_tensor.eigvecs=V; props.g_tensor.eigvals=diag(D)';
   end
   if strcmp(deblank(g03_output(n)),'g tensor (ppm):')
      line1=char(deblank(g03_output(n+1))); line2=char(deblank(g03_output(n+2))); line3=char(deblank(g03_output(n+3)));
      g=eval(['[' line1(4:15) '   ' line1(23:35) '   ' line1(42:54) ';   '...
                  line2(4:15) '   ' line2(23:35) '   ' line2(42:54) ';   '...
                  line3(4:15) '   ' line3(23:35) '   ' line3(42:54) ']']);
      g=(g+g')/2; [V,D]=eig(g); props.g_tensor.eigvecs=V; props.g_tensor.eigvals=diag(D)';
   end
   
   %% Read the chemical shielding tensors
   if strcmp(deblank(g03_output(n)),'SCF GIAO Magnetic shielding tensor (ppm):')==1
       cst=cell(natoms,1); 
       for k=1:natoms
           line1=char(g03_output(n+5*k-3)); line2=char(g03_output(n+5*k-2)); line3=char(g03_output(n+5*k-1));
           cst{k}=[eval(['[' line1(4:14) '     ' line1(21:31) '     ' line1(38:48) ']']);
                   eval(['[' line2(4:14) '     ' line2(21:31) '     ' line2(38:48) ']']);
                   eval(['[' line3(4:14) '     ' line3(21:31) '     ' line3(38:48) ']'])];
       end
       props.cst=cst; 
   end
   
   %% Read the scalar coupling matrix
   if strcmp(deblank(g03_output(n)),'Total nuclear spin-spin coupling J (Hz):')==1
       j_couplings=zeros(natoms,ceil(natoms/5)*5);
       for k=1:ceil(natoms/5)
           lines_to_read=natoms-(k-1)*5;
           current_block=zeros(natoms,5);
           for m=(n+2):(n+1+lines_to_read)
               current_line=eval(['[' char(g03_output(m)) ']']);
               current_block(current_line(1),1:(length(current_line)-1))=current_line(2:end);
           end
           j_couplings(:,(5*(k-1)+1):(5*k))=current_block;
           n=n+lines_to_read+1; %#ok<FXSET>
       end
       j_couplings=j_couplings(1:natoms,1:natoms); j_couplings=j_couplings+j_couplings';
       props.j_couplings=j_couplings;
   end
   
end
   
%% Assign atomic symbols
periodic_table={'H','He','Li','Be','B','C','N','O','F','Ne'};
props.symbols=periodic_table(atomic_numbers);

end
