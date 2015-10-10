function varargout = landscape_editor(varargin)
% LANDSCAPE_EDITOR M-file for landscape_editor.fig
%      LANDSCAPE_EDITOR, by itself, creates a new LANDSCAPE_EDITOR or raises the existing
%      singleton*.
%
%      H = LANDSCAPE_EDITOR returns the handle to a new LANDSCAPE_EDITOR or the handle to
%      the existing singleton*.
%
%      LANDSCAPE_EDITOR('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in LANDSCAPE_EDITOR.M with the given input arguments.
%
%      LANDSCAPE_EDITOR('Property','Value',...) creates a new LANDSCAPE_EDITOR or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before landscape_editor_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to landscape_editor_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2003 The MathWorks, Inc.

% Edit the above text to modify the response to help landscape_editor

% Last Modified by GUIDE v2.5 02-Mar-2014 16:00:31

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @landscape_editor_OpeningFcn, ...
                   'gui_OutputFcn',  @landscape_editor_OutputFcn, ...
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


% --- Executes just before landscape_editor is made visible.
function landscape_editor_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to landscape_editor (see VARARGIN)

% Choose default command line output for landscape_editor
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes landscape_editor wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = landscape_editor_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in popupmenu2.
function popupmenu2_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns popupmenu2 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from
%        popupmenu2


% --- Executes during object creation, after setting all properties.
function popupmenu2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function obj_x_Callback(hObject, eventdata, handles)
% hObject    handle to obj_x (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of obj_x as text
%        str2double(get(hObject,'String')) returns contents of obj_x as a double


% --- Executes during object creation, after setting all properties.
function obj_x_CreateFcn(hObject, eventdata, handles)
% hObject    handle to obj_x (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function obj_y_Callback(hObject, eventdata, handles)
% hObject    handle to obj_y (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of obj_y as text
%        str2double(get(hObject,'String')) returns contents of obj_y as a double


% --- Executes during object creation, after setting all properties.
function obj_y_CreateFcn(hObject, eventdata, handles)
% hObject    handle to obj_y (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function obj_z_Callback(hObject, eventdata, handles)
% hObject    handle to obj_z (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of obj_z as text
%        str2double(get(hObject,'String')) returns contents of obj_z as a double


% --- Executes during object creation, after setting all properties.
function obj_z_CreateFcn(hObject, eventdata, handles)
% hObject    handle to obj_z (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in btn_setobj.
function btn_setobj_Callback(hObject, eventdata, handles)
% hObject    handle to btn_setobj (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function obj_scaling_Callback(hObject, eventdata, handles)
% hObject    handle to obj_scaling (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of obj_scaling as text
%        str2double(get(hObject,'String')) returns contents of obj_scaling as a double


% --- Executes during object creation, after setting all properties.
function obj_scaling_CreateFcn(hObject, eventdata, handles)
% hObject    handle to obj_scaling (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in btn_delobj.
function btn_delobj_Callback(hObject, eventdata, handles)
% hObject    handle to btn_delobj (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in btn_addblock.
function btn_addblock_Callback(hObject, eventdata, handles)
% hObject    handle to btn_addblock (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[FileName,PathName] = uigetfile('*.txt','Road Block File');
if FileName == 0
    return
end
block = read_roadblock(sprintf('%s%s',PathName,FileName));
pts = [block.point_array.position];
left = pts(:,1:2:end)';
right = pts(:,2:2:end)';
draw_roadblock(left,right,false);

% --- Executes on button press in btn_export_landscape.
function btn_export_landscape_Callback(hObject, eventdata, handles)
% hObject    handle to btn_export_landscape (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on slider movement.
function bar_filelist_Callback(hObject, eventdata, handles)
% hObject    handle to bar_filelist (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider


% --- Executes during object creation, after setting all properties.
function bar_filelist_CreateFcn(hObject, eventdata, handles)
% hObject    handle to bar_filelist (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background, change
%       'usewhitebg' to 0 to use default.  See ISPC and COMPUTER.
usewhitebg = 1;
if usewhitebg
    set(hObject,'BackgroundColor',[.9 .9 .9]);
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in btn_addobj.
function btn_addobj_Callback(hObject, eventdata, handles)
% hObject    handle to btn_addobj (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on selection change in pop_objlist.
function pop_objlist_Callback(hObject, eventdata, handles)
% hObject    handle to pop_objlist (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns pop_objlist contents as cell array
%        contents{get(hObject,'Value')} returns selected item from pop_objlist
%set(hObject,'String',{'aabc','ccdd'});

% --- Executes during object creation, after setting all properties.
function pop_objlist_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pop_objlist (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in btn_exportobj.
function btn_exportobj_Callback(hObject, eventdata, handles)
% hObject    handle to btn_exportobj (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on selection change in listbox2.
function listbox2_Callback(hObject, eventdata, handles)
% hObject    handle to listbox2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns listbox2 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from listbox2


% --- Executes during object creation, after setting all properties.
function listbox2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to listbox2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


