const express = require('express');
const fs = require('fs');
const path = require('path');
const app = express();
const PORT = 3000;

// Middleware to parse JSON bodies
app.use(express.json());

// Load tasks from the JSON file
const tasksFile = path.join(__dirname, 'data.json');

// Helper function to load tasks
function loadTasks() {
    const data = fs.readFileSync(tasksFile, 'utf-8');
    return JSON.parse(data);
}

// Helper function to save tasks
function saveTasks(tasks) {
    fs.writeFileSync(tasksFile, JSON.stringify(tasks, null, 2), 'utf-8');
}

// Route to get all tasks
app.get('/tasks', (req, res) => {
    const tasks = loadTasks();
    res.json(tasks);
});

// Route to add a new task
app.post('/tasks', (req, res) => {
    const tasks = loadTasks();
    const newTask = { id: Date.now(), name: req.body.name };
    tasks.push(newTask);
    saveTasks(tasks);
    res.json(newTask);
});

// Route to delete a task by ID
app.delete('/tasks/:id', (req, res) => {
    const tasks = loadTasks();
    const updatedTasks = tasks.filter(task => task.id != req.params.id);
    saveTasks(updatedTasks);
    res.json({ message: 'Task deleted successfully' });
});

// Start the server
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});

