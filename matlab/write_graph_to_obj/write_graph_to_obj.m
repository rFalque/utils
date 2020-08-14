function write_graph_to_obj(graph, filename, object)
% obj_write - saves a graph struct (graph) as a wavefront obj file. 
%
% Syntax:  
%     obj_write(graph, filename)
% 
% Inputs:
%    graph - struct with two fields (FV.vertices and FV.edges) defining
%            the graph to write
%    filename - string. Name of files to be saved without file extension
%         (automatically added)
% 
% Outputs:
%    .obj files are written directly in the working directory no output 
%    into the workspace
%  
% Example: 
%    see demo.m
% 
% Author: R. Falque
% Centre for Autonomous Systems, University of Technology Sydney
% email: r.falque@gmail.com
% Created: 14-Aug-2020
% ------------- BEGIN CODE --------------

% % write .obj file
obj_filename=strcat(filename, '.obj');
fileID = fopen(obj_filename,'w'); % write new file
fprintf(fileID,'g %s\n', filename);
fprintf(fileID,'o %s\n', object);
fprintf(fileID,'v %.4f %.4f %.4f\n', graph.vertices');
fprintf(fileID,'l %d %d\n', graph.edges');
fclose(fileID);
% ------------- END OF CODE --------------
