function varargout = road_editor(varargin)
% ROAD_EDITOR M-file for road_editor.fig
%      ROAD_EDITOR, by itself, creates a new ROAD_EDITOR or raises the existing
%      singleton*.
%
%      H = ROAD_EDITOR returns the handle to a new ROAD_EDITOR or the handle to
%      the existing singleton*.
%
%      ROAD_EDITOR('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in ROAD_EDITOR.M with the given input arguments.
%
%      ROAD_EDITOR('Property','Value',...) creates a new ROAD_EDITOR or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before road_editor_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to road_editor_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2003 The MathWorks, Inc.

% Edit the above text to modify the response to help road_editor

% Last Modified by GUIDE v2.5 06-Feb-2014 12:55:51

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @road_editor_OpeningFcn, ...
                   'gui_OutputFcn',  @road_editor_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end

% End initialization code - DO NOT EDIT


% --- Executes just before road_editor is made visible.
function road_editor_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to road_editor (see VARARGIN)

% Choose default command line output for road_editor
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% set keyboard function
set(hObject,'KeyPressFcn',@MyKeyPressFcn);

global gHandles;gHandles = handles;
global gMyData;

% UIWAIT makes road_editor wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = road_editor_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


function edt_delta_angle_Callback(hObject, eventdata, handles)
% hObject    handle to edt_delta_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edt_delta_angle as text
%        str2double(get(hObject,'String')) returns contents of edt_delta_angle as a double


% --- Executes during object creation, after setting all properties.
function edt_delta_angle_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edt_delta_angle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function edt_road_width_Callback(hObject, eventdata, handles)
% hObject    handle to edt_road_width (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edt_road_width as text
%        str2double(get(hObject,'String')) returns contents of edt_road_width as a double


% --- Executes during object creation, after setting all properties.
function edt_road_width_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edt_road_width (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end

% -- plot a line
function h_line = plot_line(line,style)
if isempty(line)
    h_line = 0;
    return
end
x=[line.x];
y=[line.y];
h_line = plot(x,y,style);


% -- make a point
function pt=make_pt(x,y)
pt.x = x;
pt.y = y;

function draw_all_lines()
global gMyData;
hold on;
gMyData.h_road_edge1 = plot_line(gMyData.road_edge1,'bo');
gMyData.h_road_edge2 = plot_line(gMyData.road_edge2,'b+');
gMyData.h_mid_line = plot_line(gMyData.mid_line,'r+');

function update_axis_lim()
global gMyData;
global gHandles;
xs = [gMyData.road_edge1(end).x, gMyData.road_edge2(end).x, gMyData.min_x, gMyData.max_x];
ys = [gMyData.road_edge1(end).y, gMyData.road_edge2(end).y, gMyData.min_y, gMyData.max_y];
gMyData.min_x = min(xs);
gMyData.max_x = max(xs);
gMyData.min_y = min(ys);
gMyData.max_y = max(ys);
center.x = (gMyData.min_x+gMyData.max_x)/2;
center.y = (gMyData.min_y+gMyData.max_y)/2;
radius = max([gMyData.max_x-gMyData.min_x,gMyData.max_y-gMyData.min_y])/2;
h_axis = gHandles.road_show;
radius = radius * 1.2; % too make axis range much large for better display
xlim = [center.x-radius,center.x+radius];
ylim = [center.y-radius,center.y+radius];
set(h_axis,'XLim',xlim,'YLim',ylim);

function clear_all_lines()
cla;

function clear_all_lines_data()
global gMyData;
gMyData.road_edge1=[];
gMyData.road_edge2=[];
gMyData.mid_line=[];
gMyData.h_road_edge1=[];
gMyData.h_road_edge2=[];
gMyData.h_mid_line=[];
gMyData.road_directions=[];

function init_lines()
global gMyData;
syn_edit_data();

clear_all_lines_data();
clear_all_lines();

gMyData.direction = 90;

gMyData.road_edge1(1).x=-gMyData.half_width;
gMyData.road_edge1(1).y=0;

gMyData.road_edge2(1).x=gMyData.half_width;
gMyData.road_edge2(1).y=0;

gMyData.mid_line(1).x =0;
gMyData.mid_line(1).y=0;

gMyData.road_directions(1) = gMyData.direction;

gMyData.min_x = -gMyData.half_width;
gMyData.max_x = gMyData.half_width;
gMyData.min_y = 0;
gMyData.max_y = 0;

% -- can not call too early, when push button call it
function syn_edit_data()
global gMyData;
global gHandles;
gMyData.delta_angle = str2num(get(gHandles.edt_delta_angle,'String'));
gMyData.half_width  = str2num(get(gHandles.edt_road_width,'String'))/2;
gMyData.delta_distance  = str2num(get(gHandles.edt_delta_dist,'String'));

% --- Executes during object creation, after setting all properties.
function road_show_CreateFcn(hObject, eventdata, handles)
% hObject    handle to road_show (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on button press in btn_make_road.
function btn_make_road_Callback(hObject, eventdata, handles)
% hObject    handle to btn_make_road (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global gMyData;
gMyData.is_drawing = true;
init_lines();
update_road_show();


% --- Executes during object deletion, before destroying properties.
function btn_make_road_DeleteFcn(hObject, eventdata, handles)
% hObject    handle to btn_make_road (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)




% --- Executes on key press over figure1 with no controls selected.
function MyKeyPressFcn(hObject,event)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
syn_edit_data();
switch event.Key
    case 'uparrow'
        go_ahead();
    case 'leftarrow'
        turn_left();
    case 'rightarrow'
        turn_right();
    case 'downarrow'
        backspace();
end

% -- move point
function new_pt = move_point(pt,vec)
new_pt.x = pt.x+vec(1);
new_pt.y = pt.y+vec(2);

% -- update the figure when drawing the track
function update_road_show()
clear_all_lines();
update_axis_lim();
draw_all_lines();

% --- generate road functions
function go_ahead()
global gMyData;
deg = deg2rad(gMyData.direction);
[x,y] = pol2cart(deg,1);
vec = [x,y] * gMyData.delta_distance;
gMyData.road_edge1 = [gMyData.road_edge1,move_point(gMyData.road_edge1(end),vec)];
gMyData.road_edge2 = [gMyData.road_edge2,move_point(gMyData.road_edge2(end),vec)];
gMyData.mid_line   = [gMyData.mid_line,  move_point(gMyData.mid_line(end),vec)];
gMyData.road_directions = [gMyData.road_directions, gMyData.direction];
update_road_show();

function turn_left()
global gMyData;
gMyData.direction = gMyData.direction + gMyData.delta_angle;
deg = deg2rad(gMyData.direction);
[x,y] = pol2cart(deg-pi/2,1);
vec = [x,y]*gMyData.half_width;
gMyData.road_edge1 = [gMyData.road_edge1,gMyData.road_edge1(end)];
gMyData.road_edge2 = [gMyData.road_edge2,move_point(gMyData.road_edge1(end),vec*2)];
gMyData.mid_line   = [gMyData.mid_line,  move_point(gMyData.road_edge1(end),vec)];
gMyData.road_directions = [gMyData.road_directions, gMyData.direction];
update_road_show();

function turn_right()
global gMyData;
gMyData.direction = gMyData.direction - gMyData.delta_angle;
deg = deg2rad(gMyData.direction);
[x,y] = pol2cart(deg+pi/2,1);
vec = [x,y]*gMyData.half_width;
gMyData.road_edge1 = [gMyData.road_edge1,move_point(gMyData.road_edge2(end),vec*2)];
gMyData.road_edge2 = [gMyData.road_edge2,gMyData.road_edge2(end)];
gMyData.mid_line   = [gMyData.mid_line,  move_point(gMyData.road_edge2(end),vec)];
gMyData.road_directions = [gMyData.road_directions, gMyData.direction];
update_road_show();

function backspace()
global gMyData
gMyData.road_edge1 = gMyData.road_edge1(1:end-1);
gMyData.road_edge2 = gMyData.road_edge2(1:end-1);
gMyData.mid_line   = gMyData.mid_line(1:end-1);
gMyData.road_directions = gMyData.road_directions(1:end-1);
gMyData.direction = gMyData.road_directions(end);

xs = [gMyData.road_edge1.x,gMyData.road_edge2.x];
gMyData.min_x = min(xs);
gMyData.max_x = max(xs);
ys = [gMyData.road_edge1.y,gMyData.road_edge2.y];
gMyData.min_y = min(ys);
gMyData.max_y = max(ys);

update_road_show();

% --- Executes during object creation, after setting all properties.
function btn_make_road_CreateFcn(hObject, eventdata, handles)
% hObject    handle to btn_make_road (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
set(hObject,'KeyPressFcn',@MyKeyPressFcn);

