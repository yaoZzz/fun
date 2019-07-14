#include <python2.7/Python.h>
#include <python2.7/structmember.h>
#include <vector>
#include <string>
#include <iostream>
#include <set>

class Object
{
public:
	PyObject* GetAllState();
	
public:
    PyObject_HEAD
	std::set<std::string>* _states;
};

PyObject* Object::GetAllState()
{
	Object* self = this;
	PyObject* dict = PyDict_New();
	for (auto& s: *(self->_states)) {
		PyObject* attr = PyObject_GetAttrString((PyObject*)self, s.c_str());
		if (NULL == attr) {
			Py_XDECREF(dict);
			return NULL;
		}
		PyDict_SetItemString(dict, s.c_str(), attr);
	}
	return dict;
}

static void
Object_dealloc(Object* self)
{
	delete self->_states;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Object_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Object *self;

    self = (Object *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static int
Object_init(Object *self, PyObject *args, PyObject *kwds)
{
	self->_states = new std::set<std::string>();
    return 0;
}


static PyObject *
Object_GetState(Object* self, PyObject *args)
{
	const char* key = "";
	if(!PyArg_ParseTuple(args, "s", &key)) {
		return NULL;
	}

	if (self->_states->count(key) <= 0) {
		PyErr_SetString(PyExc_AttributeError, "state not find");
		return NULL;
	}
	
	return PyObject_GetAttrString((PyObject*)self, key);
}

static PyObject *
Object_AddStateName(Object* self, PyObject *args)
{
	const char* key = "";
	if(!PyArg_ParseTuple(args, "s", &key)) {
		return NULL;
	}
	self->_states->insert(key);
	Py_RETURN_NONE;
}

static PyObject *
Object_GetAllState(Object* self, PyObject * noarg)
{
	return self->GetAllState();
}

static PyObject *
Object_CreateObject(Object* self, PyObject * args, PyObject *keywds)
{
	//may need module info
	//PyObject* module = PyImport_ImportModule(Py_BuildValue("s", "funny"));
	PyObject* module = PyImport_ImportModule("funny");
	if (NULL == module) {
		std::cout << "not find module" << std::endl;
	} else {
		std::cout << "find module.." << std::endl;
	}
	PyObject* classType = PyObject_GetAttrString(module, "funny");
	if (NULL == classType) {
		return NULL;
	}

	if (!PyCallable_Check(classType)) {
		PyErr_SetString(PyExc_TypeError, "funny class type not callable");
		return NULL;
	}

	//PyObject *obj = PyObject_CallObject(classType, arg);
	PyObject *obj = PyObject_Call(classType, args, keywds);

	Py_XDECREF(module);
	
	return obj;
}


static PyMethodDef Object_methods[] = {
    {"get_state", (PyCFunction)Object_GetState, METH_VARARGS,      "Return the name, combining the first and last name" },
    {"get_all_state", (PyCFunction)Object_GetAllState, METH_NOARGS,      "Return the name, combining the first and last name" },
    {"add_state_name", (PyCFunction)Object_AddStateName, METH_VARARGS,      "Return the name, combining the first and last name" },
    {"create_object", (PyCFunction)Object_CreateObject, METH_VARARGS | METH_KEYWORDS,      "Return the name, combining the first and last name" },
    {NULL}  /* Sentinel */
};

static PyMemberDef Object_members[] = {
    {NULL}  /* Sentinel */
};


static PyTypeObject ObjectType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "oo.Object",             /* tp_name */
    sizeof(Object),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Object_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
	Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Object objects",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Object_methods,             /* tp_methods */
    Object_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Object_init,      /* tp_init */
    0,                         /* tp_alloc */
    Object_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

PyMODINIT_FUNC
initoo(void)
{
    PyObject* m;

    if (PyType_Ready(&ObjectType) < 0)
        return;

    m = Py_InitModule3("oo", module_methods,
                       "Example module that creates an extension type.");

    if (m == NULL)
        return;

    Py_INCREF(&ObjectType);
    PyModule_AddObject(m, "Object", (PyObject *)&ObjectType);

	std::cout << "init!!!!oo" << std::endl;
}
