#pragma once

class EditCommand
{
public:
	virtual ~EditCommand() = default;

	virtual void Undo() = 0;
	virtual void Redo() = 0;

protected:
};

